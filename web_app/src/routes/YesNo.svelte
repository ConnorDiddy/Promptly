<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import CardTitle from "../lib/CardTitle.svelte";
  
  let question = ""; // Variable to store the input value
</script>

<main>
  <Card>
    <CardTitle title="Yes / No" />
    <div class="content">
      <label for="question-input">Enter your question:</label>
      <input
        id="question-input"
        type="text"
        bind:value={question}
        placeholder="Type your question here..."
      />
      <button class="get-responses-button" on:click={() => navigate(`/results?type=yes-no&question=${encodeURIComponent(question)}`)}>
        Get Responses
      </button>
    </div>
  </Card>
</main>

<style>
  .content {
    display: flex;
    flex-direction: column;
    margin-top: 2rem;
    margin-bottom: 1rem;
    margin-inline: 40px;
  }

  input {
    margin-top: 0.5rem;
    padding: 0.5rem;
    border: 1px solid #ccc;
    border-radius: 4px;
    font-size: 1rem;
    width: 600px;
  }

  .get-responses-button {
    bottom: 1rem;
    right: 1rem;
  }
</style>
