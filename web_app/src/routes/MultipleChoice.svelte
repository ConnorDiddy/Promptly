<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import CardTitle from "../lib/CardTitle.svelte";
  import { questionText, questionAnswers, notifications } from "../stores.js";

  let answers = [
    { label: 'A', value: '' },
    { label: 'B', value: '' },
    { label: 'C', value: '' },
    { label: 'D', value: '' }
  ];

  function addAnswer() {
    const nextLabel = String.fromCharCode('A'.charCodeAt(0) + answers.length);
    // Only go up to D
    if (nextLabel === 'E') return;
    answers = [...answers, { label: nextLabel, value: '' }];
  }

  function removeAnswer(index) {
    answers = answers.filter((_, i) => i !== index).map((answer, i) => ({
      label: String.fromCharCode('A'.charCodeAt(0) + i),
      value: answer.value
    }));
  }

  function startPoll() {
    questionText.set($questionText);
    questionAnswers.set(answers.map((answer) => answer.value));
    notifications.set({});
    navigate(`/results/multiple-choice`);
  }
</script>

<main>
  <Card>
    <CardTitle title="Multiple Choice" />
    <div class="content">
      <div class="question-row">
        <label for="question-input">Question:</label>
        <input
          id="question-input"
          type="text"
          bind:value={$questionText}
          placeholder="Type your question here..."
          autocomplete="off"
        />
      </div>

      <!-- Multiple Choice Answers Section -->
      <div class="answers-section">
        {#each answers as answer, i}
          <div class="answer-row">
            <span class="answer-label">{answer.label}.</span>
            <input
              type="text"
              bind:value={answer.value}
              placeholder="Answer"
              class="answer-input"
            />
            <span class="remove-answer-button" on:click={() => removeAnswer(i)}>✖</span>
          </div>
        {/each}
        {#if answers.length < 4}
          <div class="add-answer-button" on:click={addAnswer}>+</div>
        {/if}
      </div>

      <div class="button-row">
        <button class="get-responses-button" on:click={startPoll}>
          Get Responses
        </button>
      </div>
    </div>
  </Card>
</main>

<style>
  .content {
    display: flex;
    flex-direction: column;
    min-width: 700px;
    width: 100%;
    padding: 20px;
  }

  .question-row {
    display: flex;
    flex-direction: row;
    align-items: center;
    font-weight: bold;
    font-size: 1.5rem;
  }

  .answers-section {
    margin-top: 20px;
    display: flex;
    flex-direction: column;
    align-items: center;
    width: 80%;
  }

  .answer-row {
    display: flex;
    align-items: center;
    margin-bottom: 10px;
  }

  .answer-label {
    font-weight: bold;
    margin-right: 10px;
    font-size: 1.2rem;
  }

  .add-answer-button {
    display: inline-block;
    margin-top: 10px;
    width: 30px;
    height: 30px;
    background-color: #ba1c21;
    color: white;
    border-radius: 50%;
    text-align: center;
    line-height: 30px;
    font-size: 24px;
    cursor: pointer;
  }

  .button-row {
    margin-top: 20px;
    display: flex;
    justify-content: end;
  }

  input {
    box-shadow: none;
    border-radius: 0;
    border: none;
    border-bottom: 1px solid #ba1c21;
    background-color: transparent;
    font-family: 'Altivo', sans-serif;
    height: 24px;
    margin-top: 0.5rem;
    padding: 0.5rem;
    font-size: 1rem;
    width: 100%;
  }

  label {
    margin-right: 10px;
  }

  .get-responses-button {
    bottom: 1rem;
    right: 1rem;
  }

  .remove-answer-button {
    margin-left: 10px;
    cursor: pointer;
    color: #ba1c21;
    font-size: 1.2rem;
  }
</style>
