<script lang="ts">
  import { onMount } from "svelte";
  import {
    isInitialized,
    longitude,
    latitude,
    weatherInfo,
    fetchWeatherInfo,
  } from "$lib/stores/WeatherStore";
  let isFetching = true;
  let error: GeolocationPositionError;

  onMount(() => {
    if (!$isInitialized) {
      const geolocation = navigator.geolocation;
      geolocation.getCurrentPosition(
        async (position) => {
          $longitude = position.coords.longitude;
          $latitude = position.coords.latitude;
          $weatherInfo = await fetchWeatherInfo($latitude, $longitude);
          $isInitialized = true;
        },
        (e) => {
          error = e;
          isFetching = false;
        }
      );
    }
    isFetching = false;
  });
</script>

<div class="card w-96 bg-base-100 shadow-xl">
  <div class="card-body">
    <h2 class="card-title">Weather</h2>
    {#if isFetching}
      <span class="loading flex-1 loading-spinner loading-lg self-center" />
    {:else if error}
      <p class="p-5 text-red-600">{error.message}</p>
    {:else if isInitialized}
      <div class="flex flex-row justify-start align-top">
        <div class="flex flex-col space-y-3 justify-center self-center">
          <p class="text-6xl font-semibold">
            {Math.round($weatherInfo.main.temp)}°C
          </p>
          <p class="">
            {$weatherInfo.weather[0].description} | {$weatherInfo.main
              .humidity}%
          </p>
        </div>
        <div class="flex flex-col justify-center ml-auto mb-3">
          <img
            class="w-20 object-none"
            src={`https://openweathermap.org/img/wn/${$weatherInfo.weather[0].icon}@2x.png`}
            alt=""
          />
          <p class="self-center text-sm">{$weatherInfo.name}</p>
        </div>
      </div>
    {/if}
  </div>
</div>
