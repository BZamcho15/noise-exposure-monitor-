// Phase 1: Noise Exposure Health Monitor — breadboard proof of concept
// Reads the MEMS mic on A0, finds the peak-to-peak signal swing over a
// short sampling window, and prints a rough dB estimate over Serial.
//
// NOTE: The dB value here is NOT calibrated yet — it's a relative measure
// for now. We'll calibrate it against a real sound level meter / known
// reference in a later step before it means anything in true dB SPL.

const int MIC_PIN = A0;
const unsigned long SAMPLE_WINDOW = 50; // ms — how long to sample each reading
const int NUM_WINDOWS = 5;              // how many windows to average together

unsigned int readPeakToPeak() {
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // Sample as fast as possible for SAMPLE_WINDOW milliseconds
  while (millis() - startMillis < SAMPLE_WINDOW) {
    int sample = analogRead(MIC_PIN);
    if (sample > signalMax) {
      signalMax = sample;
    }
    if (sample < signalMin) {
      signalMin = sample;
    }
  }

  return signalMax - signalMin; // 0–1023 range
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Take several peak-to-peak readings and average them.
  // This smooths out single stray spikes (like one bird chirp)
  // while still tracking real, sustained changes in loudness.
  unsigned long sum = 0;
  for (int i = 0; i < NUM_WINDOWS; i++) {
    sum += readPeakToPeak();
  }
  float avgPeakToPeak = (float)sum / NUM_WINDOWS;

  float volts = (avgPeakToPeak * 5.0) / 1024.0; // convert to volts

  // Rough relative dB estimate (will calibrate later)
  float dB = 20 * log10(volts / 0.00631);
  if (dB < 0 || isinf(dB)) {
    dB = 0; // avoid negative/garbage values when it's very quiet
  }

  Serial.print("Avg peak-to-peak: ");
  Serial.print(avgPeakToPeak);
  Serial.print("   Approx dB: ");
  Serial.println(dB);

  delay(100); // small pause so the serial monitor is readable
}
