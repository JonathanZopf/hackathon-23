<!-- AirQualityWidget.svelte -->

<script>
    const maxAIQReading = 500;
    const airQualityInfo = {
        "Excellent": "bg-success",
        "Good": "bg-success",
        "Lightly Polluted": "bg-warning",
        "Moderately Polluted": "bg-warning",
        "Heavily Polluted": "bg-error",
        "Severely Polluted": "bg-error-content",
        "Extremely Polluted": "bg-error-content",
        "Out of Range": "bg-error-content"
    };

    let airQuality = null;
    let airQualityInPercent = null;
    let airQualityQuantifier = null;
    let humidity = null;
    let temperature = null;

    async function fetchSensorData() {
        try {
            const response = await fetch("http://192.168.100.30:80/sensor-data"); // Replace with the actual API endpoint
            const data = await response.json();
            airQuality = data.iaq.toFixed(2);
            airQualityInPercent = ((airQuality / maxAIQReading) * 100).toFixed(2);
            airQualityQuantifier = getAirQualityQualifier();
            humidity = data.humidity.toFixed(2);
            temperature = data.temperature.toFixed(2);
        } catch (error) {
            console.error("Error fetching sensor data:", error);
        }

        setTimeout(fetchSensorData, 1000)
    }

    function getAirQualityQualifier() {
        if (airQuality <= 50) {
            return { name: "Excellent", color: airQualityInfo["Excellent"] };
        } else if (airQuality <= 100) {
            return { name: "Good", color: airQualityInfo["Good"] };
        } else if (airQuality <= 150) {
            return { name: "Lightly Polluted", color: airQualityInfo["Lightly Polluted"] };
        } else if (airQuality <= 200) {
            return { name: "Moderately Polluted", color: airQualityInfo["Moderately Polluted"] };
        } else if (airQuality <= 250) {
            return { name: "Heavily Polluted", color: airQualityInfo["Heavily Polluted"] };
        } else if (airQuality <= 350) {
            return { name: "Severely Polluted", color: airQualityInfo["Severely Polluted"] };
        } else if (airQuality <= 500) {
            return { name: "Extremely Polluted", color: airQualityInfo["Extremely Polluted"] };
        } else {
            return { name: "Out of Range", color: airQualityInfo["Out of Range"] };
        }
    }

    // Fetch sensor data when the component is mounted
    import { onMount } from "svelte";
    onMount(fetchSensorData);
</script>

<div class="p-4 bg-base-100">
    <div class="text-center">
        <h2 class="text-2xl font-semibold">Air Quality</h2>
        {#if airQuality !== null}
            <div class="flex items-center justify-center mt-4">
                <div class="radial-progress {airQualityQuantifier.color} text-primary-content border-4" style="--value:{airQualityInPercent};">{airQuality}</div>
            </div>
            <div class="mt-4">
                <p class="text-xl font-semibold mb-2">{airQualityQuantifier.name}</p>
                <p class="text-lg">Humidity: {humidity}%</p>
                <p class="text-lg">Temperature: {temperature}°C</p>
            </div>
        {:else}
            <span class="loading loading-spinner loading-lg" />
        {/if}
    </div>
</div>
