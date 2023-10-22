<script lang="ts">
  import GarbageWidget from "$lib/widgets/GarbageWidget.svelte";
  import RoadworksWidget from "$lib/widgets/RoadworksWidget.svelte";
  import WeatherWidget from "$lib/widgets/WeatherWidget.svelte";
  import IAQWidget from "$lib/widgets/IAQWidget.svelte";
  import JobsWidget from "$lib/widgets/JobsWidget.svelte";
  import PublicTransportWidget from "$lib/widgets/PublicTransportWidget.svelte";
  import DragAndDrop from "$lib/DragAndDrop.svelte";

  let gridAddWidgetHandler: (widget: any) => void;

  const supportedWidgets = {
    Abfallkalender: GarbageWidget,
    Baustellen: RoadworksWidget,
    Wetter: WeatherWidget,
    Luftqualität: IAQWidget,
    Arbeitsmarkt: JobsWidget,
    ÖPNV: PublicTransportWidget,
  } as const;

  function keys<T extends Record<any, any>>(obj: T): (keyof T)[] {
    return Object.keys(obj);
  }
</script>

<div class="drawer">
  <input id="my-drawer" type="checkbox" class="drawer-toggle" />
  <div class="drawer-content">
    <div class="flex flex-col">
      <label for="my-drawer" class="btn btn-circle swap swap-rotate self-end">
        <!-- this hidden checkbox controls the state -->
        <input type="checkbox" />

        <!-- hamburger icon -->
        <svg
          class="swap-off fill-current"
          xmlns="http://www.w3.org/2000/svg"
          width="32"
          height="32"
          viewBox="0 0 512 512"
          ><path
            d="M64,384H448V341.33H64Zm0-106.67H448V234.67H64ZM64,128v42.67H448V128Z"
          /></svg
        >

        <!-- close icon -->
        <svg
          class="swap-on fill-current"
          xmlns="http://www.w3.org/2000/svg"
          width="32"
          height="32"
          viewBox="0 0 512 512"
          ><polygon
            points="400 145.49 366.51 112 256 222.51 145.49 112 112 145.49 222.51 256 112 366.51 145.49 400 256 289.49 366.51 400 400 366.51 289.49 256 400 145.49"
          /></svg
        >
      </label>
    </div>
    <div class="mx-4">
      <DragAndDrop
        components={[]}
        bind:addWidgetHandler={gridAddWidgetHandler}
      />
    </div>
  </div>
  <div class="drawer-side">
    <label for="my-drawer" aria-label="close sidebar" class="drawer-overlay" />
    <div class="p-4 w-1/3 min-h-full bg-base-100 text-base-content prose">
      <h2>Widgets Hinzufügen</h2>
      <div class="divider" />
      <!-- Sidebar content here -->
      {#each keys(supportedWidgets) as widgetName}
        <div class="flex flex-row items-center px-5">
          <span class="text-xl">{widgetName}</span>
          <button
            class="btn btn-neutral ml-auto shadow hover:shadow-lg"
            on:click={() => gridAddWidgetHandler(supportedWidgets[widgetName])}
          >
            Hinzufügen
          </button>
        </div>
        <div class="divider" />
      {/each}
    </div>
  </div>
</div>
