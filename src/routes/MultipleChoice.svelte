<script>
  import { navigate } from "svelte-routing";
  import Card from "../lib/Card.svelte";
  import backArrow from "../assets/images/back-arrow.svg";
</script>

<main>
  <Card>
    <button type="button" on:click={() => navigate('/')}>
      <img src={backArrow} alt="back arrow" />
    </button>

    <h1>Multiple Choice</h1>
  </Card>
</main>

<style>

    button {
        background-color: transparent;
        border: none;
    }

    img {
        width: 50px;
    }
</style>
