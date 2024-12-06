import { writable } from "svelte/store";

export const isConnected = writable(false);
export const myCharacteristic = writable(null);
export const notifications = writable({}); 