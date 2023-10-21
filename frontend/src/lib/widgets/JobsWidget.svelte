<script lang="ts">
  import { onMount } from "svelte";
  import type { Job, Jobs } from "$lib/types/jobs.td";
  const baseURL = "https://rest.arbeitsagentur.de/jobboerse/jobsuche-service";
  let accessToken: string;
  let jobs: Jobs;

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

  async function fetchJobs(token: string) {
    const response = await fetch(
      `${baseURL}/pc/v4/app/jobs?angebotsart=1&wo=Görlitz&umkreis=200`,
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
    jobs = await fetchJobs(accessToken);
    console.log(jobs);
  });
</script>

<div class="card w-96 bg-base-100 shadow-xl">
  <div class="card-body">
    <h2 class="card-title">Jobs</h2>
    {#if jobs}
      <ul class="list-none ml-0">
        {#each jobs.stellenangebote.slice(0, 3) as job}
          <li class="p-1">
            <div class="flex flex-row justify-center">
              <p>{job.titel}</p>
              <p>{job.arbeitgeber}</p>
            </div>
          </li>
        {/each}
      </ul>
    {:else}
      <span class="loading flex-1 loading-spinner loading-xs" />
    {/if}
  </div>
</div>
