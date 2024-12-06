<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import CardHeader from "../lib/CardHeader.svelte";
  import { isConnected } from "../stores.js";
</script>

<main>
  <Card>
    <CardHeader />
    <p>Please choose a question type.</p>
    <button disabled={!$isConnected} on:click={() => navigate("/multiple-choice")}
      >Multiple Choice</button
    >
    <button disabled={!$isConnected} on:click={() => navigate("/yes-no")}>Yes / No</button>
  </Card>
</main>

<style>
  button {
    width: 350px;
    margin-bottom: 10px;
  }

  p {
    font-size: 20px;
    font-weight: bold;
  }
</style>
