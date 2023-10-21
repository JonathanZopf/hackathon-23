export interface Job {
    id: string
    beruf: string
    titel: string
    arbeitgeber: string
    arbeitsOrt: {
        plz: string
        ort: string
        region: string
        land: string
        koordinaten: {
            lat: number
            lon: number
        }
    }
    externeUrl: string
    aktuelleVeroeffentlichungsDatum: Date
    eintrittsDatum: Date
}

export interface Jobs {
    stellenangebote: Job[]
    maxErgebnisse: number
    page: number
}