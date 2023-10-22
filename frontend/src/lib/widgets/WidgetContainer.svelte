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
<div class="card card-bordered" draggable={true} on:dragstart={dragStart}>
    <div class="pt-3 pr-3">
        <button
            class="btn btn-sm btn-circle float-right"
            on:click={closeClicked}
        >
            <svg
                class="fill-current float-right"
                xmlns="http://www.w3.org/2000/svg"
                width="16"
                height="16"
                viewBox="0 0 512 512"
            >
                <polygon
                    points="400 145.49 366.51 112 256 222.51 145.49 112 112 145.49 222.51 256 112 366.51 145.49 400 256 289.49 366.51 400 400 366.51 289.49 256 400 145.49"
                />
            </svg>
        </button>
    </div>
    <div class="card-body">
        <slot />
    </div>
</div>
