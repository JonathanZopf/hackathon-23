import type { Weather } from "$lib/types/weather.td";
import { writable } from "svelte/store";

const apiKey = "4044863893e3896ee4babb891469d41b";
const baseURL = "https://api.openweathermap.org/data/2.5/weather";

export const longitude = writable(0)
export const latitude = writable(0)
export const weatherInfo = writable({} as Weather)
export const isInitialized = writable(false)

export async function fetchWeatherInfo(
    latitude: number,
    longitude: number
): Promise<Weather> {
    const response = await fetch(
        `${baseURL}?lat=${latitude}&lon=${longitude}&appid=${apiKey}&units=metric&lang=de`
    ).then((data) => data.json());
    return response as Weather;
}
