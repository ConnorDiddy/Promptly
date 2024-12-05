<script>
  import { Router, Route } from "svelte-routing";
  //@ts-ignore
  import Home from "./routes/Home.svelte";
  import MultipleChoice from "./routes/MultipleChoice.svelte";

  export let url = "";
</script>

<Router {url}>
  <main>
    <Route path="/" component={Home} />
    <Route path="/multiple-choice" component={MultipleChoice} />
    <Route path="/yes-no" component={MultipleChoice} />
  </main>
</Router>

<style>
  main {
    display: flex;
    justify-content: center;
    align-items: center;
    width: 400px;
    height: 100vh;
    margin: 0 auto;
  }
</style>
