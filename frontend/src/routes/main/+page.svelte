<script lang="ts">
    // @ts-ignore
    import Grid from "svelte-grid";
    // @ts-ignore
    import gridHelp from "svelte-grid/build/helper/index.mjs";

    let items = [
        {
            2: gridHelp.item({
                x: 0,
                y: 0,
                w: 1,
                h: 1,
            }),
            id: "Item 1",
        },

        {
            2: gridHelp.item({
                x: 1,
                y: 0,
                w: 1,
                h: 1,
            }),
            id: "Item 2",
        },

        {
            2: gridHelp.item({
                x: 0,
                y: 1,
                w: 1,
                h: 1,
            }),
            id: "Item 3",
        },

        {
            2: gridHelp.item({
                x: 1,
                y: 1,
                w: 1,
                h: 1,
            }),
            id: "Item 4",
        },
    ]

    const cols = [[0, 2]]
</script>
    <Grid {cols} bind:items={items} let:item={item} rowHeight={50} gap={[10, 10]}>
        <div class="bg-teal-400 text-center text leading-10">{item.id}</div>
    </Grid>
