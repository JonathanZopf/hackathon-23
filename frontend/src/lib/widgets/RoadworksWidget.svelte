<script lang="ts">
  import type { Roadwork } from "$lib/types/roadwork.td";
  export const fullSize = false;
  const baseURL = "https://verkehr.autobahn.de/o/autobahn";
  let roads = getRoads();
  let road = "A1";
  let roadWorks = getRoadWorks(road);

  async function getRoads() {
    const response = await fetch(baseURL).then((data) => data.json());
    return response.roads;
  }

  async function getRoadWorks(road: string): Promise<Roadwork[]> {
    const response = await fetch(`${baseURL}/${road}/services/roadworks`).then(
      (data) => data.json()
    );
    console.log(response.roadworks);
    return response.roadworks;
  }

  function getRoadWorkDelay(roadWork: Roadwork): number {
    let num = Math.floor(Math.random() * 30)
    roadWork.delay = num;
    return num
  }

  function getBadge(delay: number): string {
    if (delay == 0) return 'badge-success';
    if (delay <= 10) return 'badge-warning';
    else return 'badge-error';
  }
</script>

<div class="">
  {#if fullSize}
    <p>Test</p>
  {:else}
    <div class="card bg-gradient-to-b from-yellow-300 to-yellow-500">
      <div class="card-body">
        <div class="flex flex-row gap-5">
          <h2 class="card-title mr-auto">Roadwork</h2>
          <select
            class="select select-sm align-middle flex-auto"
            bind:value={road}
            on:change={() => {
              roadWorks = getRoadWorks(road);
            }}
          >
            {#await roads then roads}
              {#each roads as road}
                <option value={road}>{road}</option>
              {/each}
            {/await}
          </select>
        </div>
        {#await roadWorks}
          <span class="loading loading-spinner loading-lg self-center" />
        {:then roadWorks}
          <ul class="list-none list-outside p-0">
            {#each roadWorks.slice(0, 3) as roadwork}
              <li class="ml-0">
                <div class="flex flex-row gap-2">
                  <div class="flex-1 truncate">
                    {roadwork.title.split("|")[1]}
                  </div>
                  <div class="badge {getBadge(roadwork.delay)} float-right text-white">
                    +{Math.round(getRoadWorkDelay(roadwork))} Minuten
                  </div>
                </div>
                <div class="text-xs">
                  {roadwork.subtitle}
                </div>
              </li>
            {/each}
          </ul>
        {/await}
      </div>
    </div>
  {/if}
</div>
