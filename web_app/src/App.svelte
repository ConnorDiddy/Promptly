<script>
  import { Router, Route } from "svelte-routing";
  //@ts-ignore
  import Home from "./routes/Home.svelte";
  import MultipleChoice from "./routes/MultipleChoice.svelte";
  import YesNo from "./routes/YesNo.svelte";
  import YesNoResults from "./routes/YesNoResults.svelte";
  import MultipleChoiceResults from "./routes/MultipleChoiceResults.svelte";

  export let url = "";
</script>

<Router {url}>
  <main>
    <Route path="/" component={Home} />
    <Route path="/multiple-choice" component={MultipleChoice} />
    <Route path="/yes-no" component={YesNo} />
    <Route path="/results/yes-no" component={YesNoResults} />
    <Route path="/results/multiple-choice" component={MultipleChoiceResults} />
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
