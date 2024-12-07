<script>
  import utLogo from "../assets/images/ut.png";
  
</script>

<main>
  <div>
    <img src={utLogo} alt="logo" />
    <h1>Promptly</h1>
  </div>
</main>

<style>
  main {
    width: 100%;
    position: relative;
  }

  div {
    display: flex;
    flex-direction: row;
    justify-content: center;
    align-items: center;
    width: 100%;
    margin: 0;
    margin-bottom: 40px;
  }

  img {
    width: 100px;
    margin-right: 30px;
  }

  h1 {
    margin: 0;
  }
</style>
