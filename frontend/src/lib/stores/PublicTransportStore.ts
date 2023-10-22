import { writable } from "svelte/store";
export type Connection = { name: string; time: Date };

export const DISABLED = "__disabled__";

export const connections = writable([] as Connection[])
export const start = writable(DISABLED)
export const stop = writable(DISABLED)