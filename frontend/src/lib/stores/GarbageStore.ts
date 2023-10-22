import { writable } from "svelte/store";
export type Entry = { binType: string; date: Date };

export const entries = writable([] as Entry[])