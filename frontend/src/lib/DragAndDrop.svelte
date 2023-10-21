<script lang="ts">
    import {flip} from 'svelte/animate';
  import WidgetContainer from './widgets/WidgetContainer.svelte';
    export let components: any[] = [];
    let width = 0;
    let height = 0;

    const columns = Math.floor(width / 300);
    const rows = Math.floor(height / 300);

    let baskets: Record<string, any> = {};

    for (const component of components) {
        baskets[Math.random().toString()] = component;
    }

    function drop(event: DragEvent, toID: string) {
		event.preventDefault();
        const fromID = event.dataTransfer!.getData("text/plain");
		
        // switch components in baskets
        const componentInOldBasket = baskets[toID];
        baskets[toID] = baskets[fromID];
        baskets[fromID] = componentInOldBasket;
        baskets = baskets;
		
    // Add the item to the drop target basket.
	}
</script>

<div bind:clientHeight={height} bind:clientWidth={width}>
    {#each Object.keys(baskets) as basketID}
        <div class="border min-h-[200px]" ondragover="return false" on:drop={e => drop(e, basketID)}>
            <WidgetContainer {basketID}>
            <svelte:component this={baskets[basketID]}></svelte:component>
            </WidgetContainer>
        </div>
        {basketID}
    {/each}
</div>