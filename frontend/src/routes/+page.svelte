<script>
  import GarbageWidget from "$lib/widgets/GarbageWidget.svelte";
  import RoadworksWidget from "$lib/widgets/RoadworksWidget.svelte";
  import WeatherWidget from "$lib/widgets/WeatherWidget.svelte";
  import IAQWidget from "$lib/widgets/IAQWidget.svelte";
  import JobsWidget from "$lib/widgets/JobsWidget.svelte";
  import DragAndDrop from "$lib/DragAndDrop.svelte";
</script>

<div class="prose">
  <h1>Welcome to SvelteKit</h1>
  <p>
    Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation
  </p>
</div>

<DragAndDrop components={[GarbageWidget, RoadworksWidget, WeatherWidget ]} />