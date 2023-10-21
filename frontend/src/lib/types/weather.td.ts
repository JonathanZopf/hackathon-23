export interface Weather {
    coord: {
        lon: number 
        lat: number 
    }
    weather: {
        id: number
        main: string
        description: string
        icon: string
    }[]
    main: {
        temp: number
        feelsLike: number
        humidity: number
    }
    visibility: number
    wind: {
        speed: number
    }
    rain: {
        "1h": number
    }
    clouds: {
        all: number
    }
    sys: {
        type: number
        id: number 
        country: string
        sunrise: Date
        sunset: Date
    }
    timezone: string
    id: string
    name: string
    cod: number
}