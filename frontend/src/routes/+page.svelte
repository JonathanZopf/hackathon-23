<script>
    import GarbageWidget from "$lib/widgets/GarbageWidget.svelte";
    import Roadworks from "$lib/widgets/RoadworksWidget.svelte";
    import WeatherWidget from "$lib/widgets/WeatherWidget.svelte";
    import IAQWidget from "$lib/widgets/IAQWidget.svelte";
</script>

<div class="prose">
  <h1>Welcome to SvelteKit</h1>
  <p>
    Visit <a href="https://kit.svelte.dev">kit.svelte.dev</a> to read the documentation
  </p>
</div>
<div class="mockup-phone border-primary">
  <div class="camera" />
  <div class="display">
    <div class="artboard phone-1">
      <WeatherWidget/>
    </div>
  </div>
</div>
<div class="artboard phone-1">
    <IAQWidget></IAQWidget>

</div>
