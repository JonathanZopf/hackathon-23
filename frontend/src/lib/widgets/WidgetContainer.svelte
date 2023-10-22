<script lang="ts">
    export let basketID: string;

    function dragStart(event: DragEvent) {
        // The data we want to make available when the element is dropped
        // is the index of the item being dragged and
        // the index of the basket from which it is leaving.
        event.dataTransfer!.setData("text/plain", basketID);
    }
</script>

<!-- svelte-ignore a11y-no-static-element-interactions -->
<div class="card card-bordered" draggable={true} on:dragstart={dragStart}>
    <div class="card-body">
        <slot />
    </div>
</div>
