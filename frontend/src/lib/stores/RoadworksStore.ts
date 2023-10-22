import type { Roadwork } from "$lib/types/roadwork.td";
import { writable } from "svelte/store";

const baseURL = "https://verkehr.autobahn.de/o/autobahn";

export const isInitialized = writable(false);
export const roads = writable([] as string[]);
export const road = writable("A4");
export const roadWorks = writable([] as Roadwork[]);

export async function fetchRoads() {
    const response = await fetch(baseURL).then((data) => data.json());
    return response.roads;
}

export async function fetchRoadWorks(road: string): Promise<Roadwork[]> {
    const response = await fetch(`${baseURL}/${road}/services/roadworks`).then(
        (data) => data.json()
    );
    return response.roadworks;
}