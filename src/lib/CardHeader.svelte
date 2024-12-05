<script>
  import utLogo from "../assets/images/ut.png";

  let isConnected = false;

  async function handleBluetoothConnect() {
    try {
      // @ts-ignore
    //   const device = await navigator.bluetooth.requestDevice({
    //     filters: [{ services: ["battery_service"] }],
    //   });
    //   console.log("Device", device);
      isConnected = true;
    } catch (error) {
      console.error("Error", error);
    }
  }
</script>

<main>
    <img src={utLogo} alt="logo" />
    <h3>Promptly</h3>
    <button disabled={isConnected} on:click={handleBluetoothConnect}>
      {isConnected ? "Connected" : "Connect"}
    </button>
</main>

<style>
    main {
        display: flex;
        flex-direction: row;
        justify-content: space-around;
        align-items: center;
        width: 400px;
        margin: 0 auto;
    }
    
    img {
        width: 100px;
    }
    
    h3 {
        margin: 0;
    }

    button {
        border-radius: 9999px;
        padding-inline: 16px;
        padding-block: 8px;
    }
    
    button:disabled {
        background-color: #e68184;
        border: none;
        cursor: not-allowed;
    }
</style>
