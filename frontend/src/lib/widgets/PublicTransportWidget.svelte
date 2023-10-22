<script lang="ts">
    import { relativeTime, sortByTime } from "$lib/dateUtils";
    import {start, stop, DISABLED, connections} from "$lib/stores/PublicTransportStore";
    import type { Connection } from "$lib/stores/PublicTransportStore";

    const STOPS = ["Görlitz", "Bautzen", "Dresden-Neustadt", "Dresden Hbf"];
    let stopSelectState: "collapse-open" | "collapse-closed" =
        "collapse-closed";

    $connections = stopsUpdated($start, $stop);

    function toggleStopCollapse() {
        stopSelectState =
            stopSelectState === "collapse-open"
                ? "collapse-closed"
                : "collapse-open";
    }

    function stopsUpdated(start: string, stop: string): Connection[] {
        if (start === DISABLED || stop === DISABLED) return [];
        toggleStopCollapse();
        return generateRandomConnections();
    }

    function generateRandomConnections(n: number = 5): Connection[] {
        const result = [];
        for (let i = 0; i < n; i++) {
            const trainNumber = Math.floor(Math.random() * 1000) + 1000;
            const randomMinutesOffset = Math.floor(Math.random() * 300) + 3;
            const randomTime = Date.now() + randomMinutesOffset * 60 * 1000;
            result.push({
                name: "RB" + trainNumber,
                time: new Date(randomTime),
            });
        }
        return sortByTime(result);
    }
</script>

<div class="card card-bordered bg-gradient-to-b from-teal-300 to-teal-500">
    <div class="card-body">
        <h2 class="card-title">Nächste Verbindung</h2>
        <div
            class="collapse collapse-arrow border border-base-300 bg-base-200 {stopSelectState}"
        >
            <div class="collapse-title" on:click={toggleStopCollapse}>
                {#if $start === DISABLED || $stop === DISABLED}
                    Start / Stop
                {:else}
                    {$start} / {$stop}
                {/if}
            </div>
            <div class="collapse-content flex flex-col gap-2">
                <select
                    name="start-select"
                    bind:value={$start}
                    on:change={() => $connections = stopsUpdated($start, $stop)}
                    class="select select-sm shadow-sm"
                >
                    <option value={DISABLED} disabled selected>
                        Start auswählen
                    </option>
                    {#each STOPS as stop}
                        <option value={stop}>{stop}</option>
                    {/each}
                </select>
                <select
                    name="destination-select"
                    class="select select-sm shadow-sm"
                    bind:value={$stop}
                    on:change={() => $connections = stopsUpdated($start, $stop)}
                >
                    <option value={DISABLED} disabled selected>
                        Ziel auswählen
                    </option>
                    {#each STOPS as stop}
                        <option value={stop}>{stop}</option>
                    {/each}
                </select>
            </div>
        </div>
        <ol>
            {#each $connections as conn}
                <li class="p-0.5">
                    <div class="flex flex-row">
                        <span class="badge badge-neutral">{conn.name}</span>
                        <span class="ml-auto">
                            {relativeTime(conn.time)}
                        </span>
                    </div>
                </li>
            {/each}
        </ol>
    </div>
</div>
