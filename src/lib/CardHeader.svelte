<script>
  import utLogo from "../assets/images/ut.png";
  import { isConnected, myCharacteristic, notifications } from "../stores.js";

  var serviceId = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
  var characteristicId = "beb5483e-36e1-4688-b7f5-ea07361b26a8";

  async function enableNotifications(characteristic) {
    console.log(characteristic);
    if (characteristic.properties.notify) {
        characteristic.addEventListener(
          "characteristicvaluechanged",
          (event) => {
            const value = new TextDecoder().decode(event.target.value);
            console.log("Received value:", value);
            const [char, id] = value.split(":");
            notifications.update(n => {
              n[id] = char;
              return n;
            });
          }
        );

        await characteristic.startNotifications();
        console.log("Notifications started!");
      } else {
        console.log("Notifications are not supported on this characteristic.");
      }
  }

  async function handleBluetoothConnect() {
    try {
      // @ts-ignore
      navigator.bluetooth
        .requestDevice({
          filters: [{ services: [serviceId] }],
        })
        .then((device) => {
          return device.gatt.connect();
        })
        .then((server) => {
          return server.getPrimaryService(serviceId);
        })
        .then((service) => {
          return service.getCharacteristic(characteristicId);
        })
        .then((characteristic) => {
          myCharacteristic.set(characteristic);
          isConnected.set(true);
          enableNotifications(characteristic);
        });
    } catch (error) {
      console.error("Error", error);
    }
  }

  async function handleBluetoothDisconnect() {
    window.location.reload();
  }
</script>

<main>
  <button
    class={$isConnected ? "connected" : ""}
    on:click={$isConnected ? handleBluetoothDisconnect : handleBluetoothConnect}
  >
    {$isConnected ? "Disconnect" : "Connect"}
  </button>
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
    margin-top: 56px;
  }

  img {
    width: 100px;
    margin-right: 30px;
  }

  h1 {
    margin: 0;
  }

  button {
    position: absolute;
    right: 0;
    border-radius: 9999px;
    padding-inline: 16px;
    padding-block: 8px;
    border: none;
    box-shadow: none;
  }
</style>
