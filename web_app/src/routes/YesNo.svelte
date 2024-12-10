<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import CardTitle from "../lib/CardTitle.svelte";
  import { questionText } from "../stores.js";
</script>

<main>
  <Card>
    <CardTitle title="Yes or No" />
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
      <div class="button-row">
        <button
          class="get-responses-button"
          on:click={() => navigate(`/results/yes-no`)}
        >
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
</style>
