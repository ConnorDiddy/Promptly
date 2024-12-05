<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import CardHeader from "../lib/CardHeader.svelte";
</script>

<main>
  <Card>
    <CardHeader />
    <button on:click={() => navigate("/multiple-choice")}
      >Multiple Choice</button
    >
    <button on:click={() => navigate("/yes-no")}>Yes / No</button>
  </Card>
</main>

<style>
  button {
    width: 200px;
    margin-bottom: 10px;
  }
</style>
