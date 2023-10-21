const relFmt = new Intl.RelativeTimeFormat("de");
const dateFmt = new Intl.DateTimeFormat("de");
const dateAndTimeFmt = Intl.DateTimeFormat("de", {
    day: "numeric",
    month: "numeric",
    year: "numeric",
    hour: "2-digit",
    minute: "2-digit"
})

export function sortByTime<T extends { time: Date }>(array: T[]): T[] {
    return array.sort((a, b) => a.time.getTime() - b.time.getTime())
}

export function relativeTime(date: Date): string {
    const timeDiff = date.getTime() - Date.now();
    const minutesDiff = Math.floor(timeDiff / 1000 / 60);
    const hoursDiff = Math.floor(minutesDiff / 60);
    if (minutesDiff < 60) {
        return relFmt.format(minutesDiff, "minutes");
    } else if (hoursDiff < 3) {
        return relFmt.format(hoursDiff, "hours");
    } else {
        return dateAndTimeFmt.format(date);
    }
}
