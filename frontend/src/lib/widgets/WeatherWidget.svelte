<script lang="ts">
  import { onMount } from "svelte";
  import type { Weather } from "$lib/types/weather.td";
  const apiKey = "4044863893e3896ee4babb891469d41b";
  const baseURL = "https://api.openweathermap.org/data/2.5/weather";
  let longitude = 0;
  let latitude = 0;
  let isFetching = false;
  let errorWhileFetching = false;
  let weatherInfo: Weather;

  onMount(() => {
    const geolocation = navigator.geolocation;
    geolocation.getCurrentPosition(
      async (position) => {
        longitude = position.coords.longitude;
        latitude = position.coords.latitude;
        //weatherInfo = await fetchWeatherInfo(latitude, longitude);
        weatherInfo = {
          coord: { lon: 0, lat: 0 },
          weather: [
            {
              id: 0,
              main: "Clear",
              description: "Klarer Himmel",
              icon: "01d",
            },
          ],
          main: {
            temp: 17,
            feelsLike: 17,
            humidity: 72,
          },
          visibility: 10000,
          wind: { speed: 2.57 },
          clouds: { all: 0 },
          sys: {
            type: 2,
            id: 2035500,
            country: "DE",
            sunrise: new Date(1622728490),
            sunset: new Date(1622784869),
          },
          name: "Görlitz",
          cod: 200,
        } as Weather;
        isFetching = false;
      },
      () => {
        errorWhileFetching = true;
      }
    );
  });

  async function fetchWeatherInfo(
    latitude: number,
    longitude: number
  ): Promise<Weather> {
    const response = await fetch(
      `${baseURL}?lat=${latitude}&lon=${longitude}&appid=${apiKey}&units=metric&lang=de`
    ).then((data) => data.json());
    return response as Weather;
  }
</script>

<div class="card w-96 bg-base-100 shadow-xl">
  <div class="card-body">
    <h2 class="card-title">Weather</h2>
    {#if isFetching}
      <span class="loading flex-1 loading-spinner loading-xs" />
    {:else if errorWhileFetching}
      <p>Could not fetch weather information</p>
    {:else if weatherInfo}
      <div class="flex flex-row justify-start align-top">
        <div class="flex flex-col space-y-3 justify-center self-center">
          <p class="text-6xl font-semibold">{weatherInfo.main.temp}°C</p>
          <p class="">{weatherInfo.weather[0].main} | {weatherInfo.main.humidity}%</p>
        </div>
        <div class="flex flex-col justify-center ml-auto">
          <img
            class="w-20 object-none"
            src={`https://openweathermap.org/img/wn/${weatherInfo.weather[0].icon}@2x.png`}
            alt=""
          />
          <p class="self-center text-sm">Görlitz</p>
        </div>
      </div>
    {/if}
  </div>
</div>
