import { writable } from "svelte/store";

type AirQualityQuantifier = {
    name: string
    color: string
}
export const airQuality = writable(0);
export const airQualityInPercent = writable("");
export const airQualityQuantifier = writable({} as AirQualityQuantifier);
export const humidity = writable(0);
export const temperature = writable(0);