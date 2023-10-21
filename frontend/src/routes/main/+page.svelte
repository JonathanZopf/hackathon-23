<script lang="ts">
    // @ts-ignore
    import Grid from "svelte-grid";
    // @ts-ignore
    import gridHelp from "svelte-grid/build/helper/index.mjs";

    function createItem(x:number, y:number) {
        return {
            2: gridHelp.item({
                x: x,
                y: y,
                w: 1,
                h: 1,
                resizable: false,
            }),
            id: Math.random().toString().substring(2),
        }
    }

    let items = [
        createItem(0, 0),
        createItem(1, 0),
    ]

    const cols = [[0, 2]]



    function addItemToGrid() {
        let newItem = createItem(0, 0);
        let freeSpace = gridHelp.findSpace(newItem, items, 2);

        newItem = {
            ...newItem,
            [2]: {
                ...newItem[2],
                ...freeSpace,
            },
        };

        items = [...items, newItem];
    }

    /**
     * Get the highest y value within the grid
     * The item provided is excluded from the search
     * @param item Item to exclude
     */
    function getMaxY(item:any) {
        let maxY = 0;
        for (let i = 0; i < items.length; i++) {
            if (items[i].id === item.id) continue;
            if (items[i][2].x == item[2].x && items[i][2].y > maxY) {
                maxY = items[i][2].y;
            }
        }
        return maxY + 1;
    }

    /**
     * Check if an item is free floating, if so, limit it's y value to be a neighbor of another one
     */
    function checkIfFreeFloating() {
        for (const item of items) {
            //Check if has no top neighbor (is free floating)
            if (item[2].y > getMaxY(item)) {
                //Limit y and force refresh
                item[2].y = getMaxY(item);
                items = items;
            }
        }
    }
</script>
    <Grid {cols} bind:items={items} let:dataItem rowHeight={50} gap={[10, 10]} on:change="{checkIfFreeFloating}">
        <div class="dragger bg-teal-400 text-center text leading-10">{dataItem.id}</div>

    </Grid>
<button class="btn btn-circle" on:click={addItemToGrid}>
    <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M6 18L18 6M6 6l12 12" /></svg>
</button>