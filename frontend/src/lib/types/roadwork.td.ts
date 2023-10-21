export interface Roadwork {
	extent: string
	identifier: string
	routeRecommendations: []
	coordinate: {lat: string, long: string}
	footer: string[]
	impact: {
		lower: string,
		upper: string,
		symbols: string[]
	}
	icon: string
	isBlocked: boolean
	description: string[]
	title: string
	point: string
	displayType: string
	future: boolean
	subtitle: string
	startTimeStamp: Date
	delay: number
}