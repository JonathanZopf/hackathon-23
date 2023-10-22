<script lang="ts">
  import { onMount } from "svelte";
  import type { Job, Jobs } from "$lib/types/jobs.td";
  const baseURL = "https://rest.arbeitsagentur.de/jobboerse/jobsuche-service";
  let accessToken: string;
  let steve: Jobs;
  let jobType: number;

  const jobTypes = {
    1: "Vollzeit",
    2: "Selbständig",
    4: "Ausbildung/Duales Studium",
    34: "Praktikum"
  }

  async function fetchAccessToken() {
    const response = await fetch(
      "https://rest.arbeitsagentur.de/oauth/gettoken_cc",
      {
        method: "POST",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: "client_secret=32a39620-32b3-4307-9aa1-511e3d7f48a8&client_id=c003a37f-024f-462a-b36d-b001be4cd24a&grant_type=client_credentials",
      }
    ).then((r) => r.json());
    return response.access_token as string;
  }

  async function fetchJobs(token: string, type: number = 1) {
    const response = await fetch(
      `${baseURL}/pc/v4/app/jobs?angebotsart=${type}&wo=Görlitz&umkreis=200`,
      {
        headers: {
          OAuthAccesstoken: accessToken,
        },
      }
    ).then((r) => r.json());
    return response as Jobs;
  }

  onMount(async () => {
    accessToken = await fetchAccessToken();
    steve = await fetchJobs(accessToken);
    console.log(steve);
  });
</script>

<div class="card bg-gradient-to-b from-green-300 to-green-500">
  <div class="card-body">
    {#if steve}
      <div class="flex flex-row align-center gap-5">
        <h2 class="card-title m-0">Jobs</h2>
        <div class="badge badge-success float-rigt self-center mt-1">
          {steve.stellenangebote.length}
        </div>
        <select class="select select-sm self-center flex-auto"
                bind:value={jobType}
                on:change={async () => {
                  steve = await fetchJobs(accessToken, jobType);
                }}>
              {#each Object.keys(jobTypes) as type}
                <option value={type}>{jobTypes[type]}</option>
              {/each}
              </select>
      </div>
      <ul class="list-none list-outside p-0">
        {#each steve.stellenangebote.slice(0, 3) as job}
          <li class="ml-0">
            <div class="flex flex-col align-left">
              <div class="flex-1 truncate">{job.titel}</div>
              <a class="text-sm font-semibold underline" href={job.externeUrl}>{job.arbeitgeber}</a>
            </div>
          </li>
            <div class="divider lg:divider-horizontal m-0"></div> 
        {/each}
      </ul>
    {:else}
      <span class="loading loading-spinner loading-ls self-center" />
    {/if}
  </div>
</div>
