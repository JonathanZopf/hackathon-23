<script lang="ts">
    // @ts-ignore
    import Grid from "svelte-grid";
    // @ts-ignore
    import gridHelp from "svelte-grid/build/helper/index.mjs";
    import GarbageWidget from "$lib/widgets/GarbageWidget.svelte";
    import PublicTransportWidget from "$lib/widgets/PublicTransportWidget.svelte";

    const cols = [[1000, 1], [5000, 2]]

    let columnWidth:number;

    function getColumnCount() {
        for (let item in cols) {
            if (columnWidth < cols[item][0]) {
                return cols[item][1];
            }
        }
        throw new Error("No column count found");
    }

    function createItem(x:number, y:number, component:any) {
        return {
            1: gridHelp.item({
                x: 0,
                y: y,
                w: 1,
                h: 1,
                resizable: false,
            }),
            2: gridHelp.item({
                x: x,
                y: y,
                w: 1,
                h: 1,
                resizable: false,
            }),
            id: Math.random().toString().substring(2),
            data: component,
        }
    }

    let items = [
        createItem(0, 0, PublicTransportWidget),
        createItem(1, 0, GarbageWidget),
    ]

    function addItemToGrid() {
        const columnCount = getColumnCount();
        let newItem = createItem(0, 0, GarbageWidget);
        let freeSpace = gridHelp.findSpace(newItem, items, columnCount);

        newItem = {
            ...newItem,
            [columnCount]: {
                //@ts-ignore
                ...newItem[columnCount],
                ...freeSpace,
            },
        };

        items = [...items, newItem];
    }

    /**
     * Get the highest y value within the grid
     * The item provided is excluded from the search
     * @param item Item to exclude
     * @param columnCount Number of columns in the grid
     */
    function getMaxY(item:any, columnCount:number) {
        let maxY = 0;
        for (let i = 0; i < items.length; i++) {
            if (items[i].id === item.id) continue;
            //@ts-ignore
            if (items[i][columnCount].x == item[columnCount].x && items[i][columnCount].y > maxY) {
                //@ts-ignore
                maxY = items[i][columnCount].y;
            }
        }
        return maxY + 1;
    }

    /**
     * Check if an item is free floating, if so, limit it's y value to be a neighbor of another one
     */
    function checkIfFreeFloating() {
        let columnCount = getColumnCount();
        for (const item of items) {
            //Check if has no top neighbor (is free floating)
            //@ts-ignore
            if (item[columnCount].y > getMaxY(item, columnCount)) {
                //Limit y and force 0refresh
                //@ts-ignore
                item[columnCount].y = getMaxY(item, columnCount);
                items = items;
            }
        }
    }
</script>
<div bind:clientWidth={columnWidth}>
    <Grid {cols} bind:items={items} let:dataItem rowHeight={250} gap={[10, 10]} on:change="{checkIfFreeFloating}">
        <svelte:component this="{dataItem.data}"></svelte:component>
    </Grid>
</div>
<button class="btn btn-circle" on:click={addItemToGrid}>
    <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" /></svg>
</button>