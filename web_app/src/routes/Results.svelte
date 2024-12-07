<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import CardTitle from "../lib/CardTitle.svelte";
  import { notifications } from "../stores.js";
  import CardHeader from "../lib/CardHeader.svelte";

  let question = "";
  let type = "";
  let yesCount = 0;
  let noCount = 0;

  const urlParams = new URLSearchParams(window.location.search);
  type = urlParams.get("type");
  question = urlParams.get("question");

  // Subscribe to notifications and update counts
  notifications.subscribe((value) => {
    if (type === "yes-no") {
      yesCount = 0;
      noCount = 0;
      for (const id in value) {
        if (value[id] === "1") {
          yesCount++;
        } else if (value[id] === "0") {
          noCount++;
        }
      }
    }
  });

  function handleEndPoll() {
    navigate("/");
    // Reset the notifications store
    notifications.update((n) => {
      for (const id in n) {
        n[id] = "";
      }
      return n;
    });
  }
</script>

<main>
  <Card>
    <CardTitle title={question} />
    {#if type === "yes-no"}
      <div class="poll-container">
        <div class="answer-row">
          <div class="answer-label">
            <div class="option yes">Yes</div>
          </div>
          <div class="answer-count">{yesCount}</div>
          <div class="answer-bar">
            <div class="bar" style="width: {yesCount * 10}px;"></div>
          </div>
        </div>

        <div class="answer-row">
          <div class="answer-label">
            <div class="option no">No</div>
          </div>
          <div class="answer-count">{noCount}</div>
          <div class="answer-bar">
            <div class="bar" style="width: {noCount * 10}px;"></div>
          </div>
        </div>
      </div>
      <div class="action-row">
        <button class="end-poll-button" on:click={handleEndPoll}
          >End Poll</button
        >
      </div>
    {/if}
  </Card>
</main>

<style>
  main {
    display: flex;
    justify-content: center;
    margin-top: 2rem;
  }

  .poll-container {
    display: flex;
    flex-direction: column;
    align-items: start;
    gap: 1rem;
    margin: 3.5rem 15px;
  }

  .answer-row {
    display: flex;
    align-items: center;
    justify-content: start;
    gap: 1rem;
  }

  .answer-label {
    flex: 0 0 auto;
  }

  .option {
    width: 100px;
    background-color: #ba1c21;
    color: #fff;
    border: none;
    padding: 8px 100px;
    font-weight: bold;
    border-radius: 4px;
    display: flex;
    justify-content: center;
  }

  .answer-count {
    flex: 0 0 auto;
    font-size: 1.2rem;
    font-weight: bold;
  }

  .answer-bar {
    flex: 1;
    background: transparent;
    height: 1.5rem;
    overflow: hidden;
    position: relative;
  }

  .bar {
    height: 100%;
    background-color: #003058;
  }

  .action-row {
    display: flex;
    flex-direction: row;
    justify-content: end;
    width: 100%;
  }

  .end-poll-button {
    background-color: #ba1c21;
    color: #fff;
    border: none;
    padding: 8px 48px;
    font-weight: bold;
    border-radius: 4px;
    cursor: pointer;
  }
</style>
