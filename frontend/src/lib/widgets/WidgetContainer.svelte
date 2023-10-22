<script lang="ts">
    import { createEventDispatcher } from "svelte";
    export let basketID: string;

    const dispatch = createEventDispatcher();

    function dragStart(event: DragEvent) {
        // The data we want to make available when the element is dropped
        // is the index of the item being dragged and
        // the index of the basket from which it is leaving.
        event.dataTransfer!.setData("text/plain", basketID);
    }

    function closeClicked() {
        dispatch("close");
    }
</script>

<!-- svelte-ignore a11y-no-static-element-interactions -->
<div class="" draggable={true} on:dragstart={dragStart}>
    <slot />
</div>
