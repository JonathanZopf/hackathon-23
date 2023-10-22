<script lang="ts">
  import {
    fetchAccessToken,
    jobTypes,
    steve,
    fetchJobs,
    jobType,
    accessToken,
    isInitialized,
  } from "$lib/stores/JobsStore";
  import type { Jobs } from "$lib/types/jobs.td";
  import { onMount } from "svelte";

  onMount(async () => {
    if (location.hostname === "hostname") {
      $accessToken = await fetchAccessToken();
      localStorage.setItem("accessToken", $accessToken);
      $steve = await fetchJobs($jobType);
    }
    $steve = {
      stellenangebote: [
        {
          beruf: "Eisenbahner/in - Betrieb. - Lok. u. Transp. (Ausb. bis 2022)",
          titel: "Triebfahrzeugführer (m/w/d) für unsere Standorte Dresden und",
          arbeitgeber: "Netinera Deutschland Gmbh",
          externeUrl: "http://anzeigen.schienenjobs.de/?anzeige=2202479020",
        },
        {
          beruf: "Zerspanungsmechaniker/in",
          titel: "Zerspanungsmechaniker (m/w/d) Frästechnik in Tagschicht",
          arbeitgeber: "Akzent Personaldienstleistungen GmbH",
        },
        {
          beruf: "Anwendungsprogrammierer/in",
          titel: "Svelte Developer (m/w/d)",
          arbeitgeber: "NAVAX Software",
          externeUrl:
            "https://www.get-in-it.de/jobsuche/p235547?utm_source=arbeitsagentur&utm_medium=cpc&utm_campaign=launch-basic",
        },
      ],
    } as Jobs;
    $isInitialized = true;
  });
</script>

<div class="card bg-gradient-to-b from-green-300 to-green-500">
  <div class="card-body">
    {#if $isInitialized}
      <div class="flex flex-row align-center gap-5">
        <h2 class="card-title m-0">Jobs</h2>
        <div class="badge badge-success float-rigt self-center mt-1">
          {$steve.stellenangebote.length}
        </div>
        <select
          class="select select-sm self-center flex-auto"
          bind:value={$jobType}
          on:change={async () => {
            $steve = await fetchJobs($jobType);
          }}
        >
          {#each Object.keys($jobTypes) as type}
            <option value={type}>{$jobTypes[type]}</option>
          {/each}
        </select>
      </div>
      <ul class="list-none list-outside p-0">
        {#each $steve.stellenangebote as job}
          <li class="mt-1">
            <div class="flex flex-col align-left">
              <div class="flex-1 truncate">{job.titel}</div>
              <a class="text-sm font-semibold underline" href={job.externeUrl} target="_blank"
                >{job.arbeitgeber}</a
              >
              <div class="divider m-0" />
            </div>
          </li>
        {/each}
      </ul>
    {:else}
      <span class="loading loading-spinner loading-ls self-center" />
    {/if}
  </div>
</div>
