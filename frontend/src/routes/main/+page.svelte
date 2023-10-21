<script lang="ts">
    // @ts-ignore
    import Grid from "svelte-grid";
    // @ts-ignore
    import gridHelp from "svelte-grid/build/helper/index.mjs";
    import GarbageWidget from "$lib/widgets/GarbageWidget.svelte";
    import PublicTransportWidget from "$lib/widgets/PublicTransportWidget.svelte";
    import WeatherWidget from "$lib/widgets/WeatherWidget.svelte";
    import RoadworksWidget from "$lib/widgets/RoadworksWidget.svelte";
    import IAQWidget from "$lib/widgets/IAQWidget.svelte";

    const cols = [[600, 1], [5000, 2]]

    let columnWidth:number;

    function getColumnCount() {
        for (let item in cols) {
            if (columnWidth <= cols[item][0]) {
                return cols[item][1];
            }
        }
        throw new Error("No column count found");
    }

    function createItem(x:number, y:number, component:any) {
        return {
            1: gridHelp.item({
                x: 0,
                y: 2 * y + x,
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
        let maxY = -1; //If there is no item maxY is -1
        for (let i = 0; i < items.length; i++) {
            if (items[i].id === item.id) continue;
            //@ts-ignore
            if (items[i][columnCount].x == item[columnCount].x && items[i][columnCount].y > maxY) {
                //@ts-ignore
                maxY = items[i][columnCount].y;
            }
        }
        return maxY;
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
                //Limit y and force refresh
                //@ts-ignore
                item[columnCount].y = getMaxY(item, columnCount) + 1
                items = items;
            }
        }
    }
</script>
<div class="drawer">
    <input id="my-drawer" type="checkbox" class="drawer-toggle" />
    <div class="drawer-content">
        <div bind:clientWidth={columnWidth}>
            <Grid {cols} bind:items={items} let:dataItem rowHeight={250} gap={[10, 10]} on:change="{checkIfFreeFloating}">
                <svelte:component this="{dataItem.data}"></svelte:component>
            </Grid>
        </div>
        <div class="flex flex-col">
        <label for="my-drawer" class="btn btn-circle swap swap-rotate self-end">

            <!-- this hidden checkbox controls the state -->
            <input type="checkbox" />

            <!-- hamburger icon -->
            <svg class="swap-off fill-current" xmlns="http://www.w3.org/2000/svg" width="32" height="32" viewBox="0 0 512 512"><path d="M64,384H448V341.33H64Zm0-106.67H448V234.67H64ZM64,128v42.67H448V128Z"/></svg>

            <!-- close icon -->
            <svg class="swap-on fill-current" xmlns="http://www.w3.org/2000/svg" width="32" height="32" viewBox="0 0 512 512"><polygon points="400 145.49 366.51 112 256 222.51 145.49 112 112 145.49 222.51 256 112 366.51 145.49 400 256 289.49 366.51 400 400 366.51 289.49 256 400 145.49"/></svg>

        </label>
        </div>
    </div>
    <div class="drawer-side">
        <label for="my-drawer" aria-label="close sidebar" class="drawer-overlay"></label>
        <div class="menu p-4 w-1/3 min-h-full bg-base-100 text-base-content">
            <!-- Sidebar content here -->
            <div class="flex flex-col gap-4">
                <GarbageWidget on:click={addItemToGrid}></GarbageWidget>
                <PublicTransportWidget></PublicTransportWidget>
                <WeatherWidget></WeatherWidget>
                <RoadworksWidget></RoadworksWidget>
                <IAQWidget></IAQWidget>
            </div>
        </div>
    </div>
</div>
