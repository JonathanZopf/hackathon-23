<script lang="ts">
  import type { Roadwork } from "$lib/types/roadwork.td";
  import { onMount } from "svelte";
  import {
    fetchRoadWorks,
    fetchRoads,
    isInitialized,
    roads,
    road,
    roadWorks,
  } from "$lib/stores/RoadworksStore";

  onMount(async () => {
    if (!$isInitialized) {
      $roads = await fetchRoads();
      $roadWorks = await fetchRoadWorks($road);
      $isInitialized = true;
    }
  });

  function getRoadWorkDelay(roadWork: Roadwork): number {
    let num = Math.floor(Math.random() * 30);
    roadWork.delay = num;
    return num;
  }

  function getBadge(delay: number): string {
    if (delay == 0) return "badge-success";
    if (delay <= 10) return "badge-warning";
    else return "badge-error";
  }
</script>

<div class="card card-compact bg-slate-600 text-primary-content">
  <div class="card-body">
    <div class="flex flex-row gap-5">
      <h2 class="card-title my-0">Baustellen</h2>
      {#if $isInitialized}
        <select
          class="select select-sm align-middle flex-auto"
          bind:value={$road}
          on:change={async () => {
            $roadWorks = await fetchRoadWorks($road);
          }}
        >
          {#each $roads as road}
            <option value={road}>{road}</option>
          {/each}
        </select>
      {/if}
    </div>
    {#if !$isInitialized}
      <span class="loading loading-spinner loading-lg self-center" />
    {:else}
      <ul class="list-none list-outside p-0">
        {#each $roadWorks.slice(0, 3) as roadwork}
          <li class="ml-0">
            <div class="flex flex-row gap-2">
              <div class="flex-1 truncate">
                {roadwork.title.split("|")[1]}
              </div>
              <div
                class="badge {getBadge(roadwork.delay)} float-right text-white"
              >
                +{Math.round(getRoadWorkDelay(roadwork))} Minuten
              </div>
            </div>
            <div class="text-xs text-gray-300 ml-1 mt-1">
              {roadwork.subtitle}
            </div>
          </li>
        {/each}
      </ul>
    {/if}
  </div>
</div>
