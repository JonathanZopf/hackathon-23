<script lang="ts">
    // @ts-ignore
    import ical from "ical.js";
    import { onMount } from "svelte";

    type Entry = { binType: string; date: Date };
    let entries: Entry[] = [];

    onMount(async () => {
        const response = await fetch("/garbage-dates.ics");
        const icalString = await response.text();

        const parsed = ical.parse(icalString);
        const vcal = new ical.Component(parsed);
        const events = vcal.getAllSubcomponents("vevent");
        const asEntries: Entry[] = events.map((e: any) => {
            const date = e.getFirstPropertyValue("dtstart").toJSDate();
            const binType = e.getFirstPropertyValue("summary");
            return { date, binType } as Entry;
        });
        entries = asEntries
            .sort((a, b) => a.date.getTime() - b.date.getTime())
            .filter((entry) => entry.date.getTime() > Date.now());
    });

    function isToday({ date }: Entry): boolean {
        const today = new Date();
        return (
            date.getFullYear() === today.getFullYear() &&
            date.getMonth() === today.getMonth() &&
            date.getDate() === today.getDate()
        );
    }

    function truncateString(str: string, maxLen: number): string {
        if (str.length < maxLen) return str;
        return str.slice(0, maxLen - 3) + "...";
    }

    const dayOfYear = (date: Date) => {
        return Math.floor(
            // @ts-ignore
            (date - new Date(date.getFullYear(), 0, 0)) / 1000 / 60 / 60 / 24
        );
    };

    const relativeFmt = new Intl.RelativeTimeFormat("de");
    const dateFmt = new Intl.DateTimeFormat("de");

    function relativeTime(date: Date): string {
        const daysDiff = dayOfYear(date) - dayOfYear(new Date());
        return daysDiff < 10
            ? relativeFmt.format(daysDiff, "days")
            : dateFmt.format(date);
    }
</script>

<div class="card card-bordered bg-base-100">
    <div class="card-body">
        <div class="flex flex-row">
            <h2 class="card-title mr-auto">Abfallkalender</h2>
            <svg
                xmlns="http://www.w3.org/2000/svg"
                height="24"
                viewBox="0 -960 960 960"
                width="24"
                class="fill-base-content"
                ><path
                    d="M200-200h57l391-391-57-57-391 391v57Zm-80 80v-170l528-527q12-11 26.5-17t30.5-6q16 0 31 6t26 18l55 56q12 11 17.5 26t5.5 30q0 16-5.5 30.5T817-647L290-120H120Zm640-584-56-56 56 56Zm-141 85-28-29 57 57-29-28Z"
                /></svg
            >
        </div>
        {#if entries.length === 0}
            <span class="loading loading-spinner loading-sm" />
        {/if}
        <ol>
            {#each entries.slice(0, 5) as entry}
                <li>
                    {truncateString(entry.binType, 20)}
                    <div
                        class="badge {isToday(entry)
                            ? 'badge-warning'
                            : 'badge-neutral'} float-right"
                    >
                        {relativeTime(entry.date)}
                    </div>
                </li>
            {/each}
        </ol>
    </div>
</div>
