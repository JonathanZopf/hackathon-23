import type { Jobs } from "$lib/types/jobs.td";
import { readable, writable, get } from "svelte/store";

const baseURL = "https://rest.arbeitsagentur.de/jobboerse/jobsuche-service";

export let accessToken = writable("")
export let isInitialized = writable(false);
export const steve = writable({} as Jobs)
export const jobTypes = readable({
    1: "Vollzeit",
    2: "Selbständig",
    4: "Ausbildung/Duales Studium",
    34: "Praktikum",
})
export const jobType = writable(1);

export async function fetchAccessToken() {
    const response = await fetch(
        "https://rest.arbeitsagentur.de/oauth/gettoken_cc",
        {
            method: "POST",
            headers: {
                "Content-Type": "application/x-www-form-urlencoded",
            },
            body: "client_secret=32a39620-32b3-4307-9aa1-511e3d7f48a8&client_id=c003a37f-024f-462a-b36d-b001be4cd24a&grant_type=client_credentials",
        }
    ).then((r) => r.json());
    return response.access_token as string;
}

export async function fetchJobs(type: number = 1) {
    const response = await fetch(
        `${baseURL}/pc/v4/app/jobs?angebotsart=${type}&wo=Görlitz&umkreis=200`,
        {
            headers: {
                OAuthAccesstoken: get(accessToken),
            },
        }
    ).then((r) => r.json());
    return response as Jobs;
}