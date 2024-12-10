<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import CardTitle from "../lib/CardTitle.svelte";
  import { notifications, questionText, questionAnswers } from "../stores.js";

  let counts = { A: 0, B: 0, C: 0, D: 0 };

  // Subscribe to notifications and update counts
  notifications.subscribe((value) => {
    counts = { A: 0, B: 0, C: 0, D: 0 };
    for (const id in value) {
      if (value[id] === "A") {
        counts.A++;
      } else if (value[id] === "B") {
        counts.B++;
      } else if (value[id] === "C") {
        counts.C++;
      } else if (value[id] === "D") {
        counts.D++;
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
    questionText.set("");
    questionAnswers.set([]);
  }
</script>

<main>
  <Card>
    <CardTitle title={$questionText} />
    <div class="poll-container">
      {#each Object.entries(counts) as [label, count]}
        <div class="answer-row">
          {label}.
          {#if $questionAnswers[label?.charCodeAt(0) - 'A'.charCodeAt(0)]}
          <div class="answer-label">
            <div class="option">{$questionAnswers[label?.charCodeAt(0) - 'A'.charCodeAt(0)]}</div>
          </div>
          {/if}
          <div class="answer-count">{count}</div>
          <div class="answer-bar">
            <div class="bar" style="width: {count * 10}px;"></div>
          </div>
        </div>
      {/each}
    </div>
    <div class="action-row">
      <button class="end-poll-button" on:click={handleEndPoll}>End Poll</button>
    </div>
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
    margin: 2.5rem ;
  }

  .answer-row {
    display: flex;
    align-items: center;
    justify-content: start;
    gap: 1rem;
    font-weight: bold;
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
    white-space: nowrap;
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
