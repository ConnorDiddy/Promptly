<script>
    import { navigate } from "svelte-routing";
    import backArrow from "../assets/images/back-arrow.svg";
    export let title = "";
  </script>
  
  <main>
    <button type="button" on:click={() => history.back()}>
      <img src={backArrow} alt="back arrow" />
    </button>
    <h1>{title}</h1>
  </main>
  
  <style>
    main {
      position: relative;
      display: flex;
      align-items: center;
      justify-content: center;
      margin: 0;
      padding: 0;
      width: 100%;
    }
  
    button {
      position: absolute;
      left: 0;
      background: transparent;
      border: none;
      margin: 0;
      padding: 0;
    }
  
    h1 {
      font-size: 1.5rem;
      font-weight: bold;
      margin: 0;
      text-align: center;
    }
  
    img {
      width: 50px;
    }
  </style>
  