<script>
  import { onMount } from "svelte";
  import { articles } from "$lib/stores/NewsStore";

  let themes = [
    "Inland",
    "Ausland",
    "Wirtschaft",
    "Sport",
    "Video",
    "Investigativ",
    "Faktenfinder",
  ];
  let selectedTheme = "Inland"; // The selected theme for news articles

  const fetchNews = async () => {
    const response = await fetch(
      `https://www.tagesschau.de/api2/news/?regions=13&ressort=${selectedTheme}`
    );
    const data = await response.json();
    $articles = data.news.slice(0, 4); // Display the first 4 news articles
  };

  // Handle the dropdown selection
  const handleThemeChange = (e) => {
    selectedTheme = e.target.querySelector("option:checked")?.__value;
    fetchNews();
  };

  onMount(fetchNews);
</script>

<h2 class="card-title mr-auto text-center">News</h2>

<!-- Dropdown menu -->
<select
  name="news-select"
  on:change={handleThemeChange}
  class="select select-sm shadow-sm w-full"
>
  {#each themes as theme}
    <option value={theme}>{theme}</option>
  {/each}
</select>

<!-- List of news articles -->
<div class="news-widget">
  {#if $articles.length > 0}
    <div class="grid grid-cols-2 gap-4">
      {#each $articles as article (article.sophoraId)}
        <div class="card card-compact card-bordered">
          {#if article.teaserImage}<figure>
              <img
                class="rounded w-full h-20"
                src={article.teaserImage.imageVariants["16x9-256"]}
                alt={article.teaserImage.alttext}
              />
            </figure>
          {/if}
          <div class="card-body">
            <p class="text-xs">{article.title}</p>
          </div>
        </div>
      {/each}
    </div>
  {:else}
    <p>No articles available for the selected theme.</p>
  {/if}
</div>
