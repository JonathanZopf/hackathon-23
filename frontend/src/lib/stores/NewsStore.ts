import { writable } from "svelte/store"

export type Article = {
    sophoraId: string
    teaserImage: {
        alttext: string
        imageVariants: Record<string,string>
    }
    title: string
}

export const articles = writable([] as Article[])