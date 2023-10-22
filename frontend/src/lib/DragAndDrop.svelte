<script lang="ts">
    import WidgetContainer from "./widgets/WidgetContainer.svelte";
    export let components: any[] = [];
    let width = 0;
    let height = 0;

    const columns = Math.floor(width / 300);
    const rows = Math.floor(height / 300);

    let baskets: Record<string, any> = {};

    for (const component of components) {
        baskets[Math.random().toString()] = component;
    }

    let currentDragOverID: string | null = null;

    function drop(event: DragEvent, toID: string) {
        currentDragOverID = null;
        event.preventDefault();
        const fromID = event.dataTransfer!.getData("text/plain");

        // switch components in baskets
        const componentInOldBasket = baskets[toID];
        baskets[toID] = baskets[fromID];
        baskets[fromID] = componentInOldBasket;
        baskets = baskets;

        // Add the item to the drop target basket.
    }

    export function addWidgetHandler(widget: any) {
        baskets[Math.random().toString()] = widget;
        baskets = baskets;
    }
</script>

<div
    bind:clientHeight={height}
    bind:clientWidth={width}
    class="flex flex-row flex-wrap gap-3 align-top"
>
    {#each Object.keys(baskets) as basketID (basketID)}
        <!-- svelte-ignore a11y-no-static-element-interactions -->
        <div
            class="min-h-[200px] grow {currentDragOverID === basketID
                ? 'border rounded-2xl border-warning'
                : ''}"
            ondragover="return false"
            on:drop={(e) => drop(e, basketID)}
            on:dragenter={() => (currentDragOverID = basketID)}
            on:dragleavge={() => (currentDragOverID = null)}
        >
            <WidgetContainer {basketID}>
                <svelte:component this={baskets[basketID]} />
            </WidgetContainer>
        </div>
    {/each}
</div>
