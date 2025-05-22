# How It Works

- **Startup:**  
  Initializes all hardware, connects to WiFi, and displays a welcome message.
- **Lid Control:**  
  When someone is detected within 30 cm, the servo opens the lid for 5 seconds.
- **Fill Level:**  
  The internal sensor measures distance to trash; fill percent is shown on the OLED.
- **Alert:**  
  If the bin is over 80% full, an email is sent and an alert appears on the display.

## Example Workflow (TypeScript Pseudocode)

Here's a conceptual flow of what the Arduino code does, for illustration:

```typescript
type BinStatus = 'open' | 'closed';

class SmartTrashBin {
  fillLevel: number; // percent
  lidStatus: BinStatus;
  emailSent: boolean;

  constructor() {
    this.fillLevel = 0;
    this.lidStatus = 'closed';
    this.emailSent = false;
  }

  async monitor() {
    await this.connectWiFi();
    setInterval(() => {
      this.checkProximity();
      this.checkFillLevel();
      this.updateDisplay();
    }, 1000);
  }

  checkProximity() {
    if (this.detectPersonNearby() && this.lidStatus === 'closed') {
      this.openLid();
      setTimeout(() => this.closeLid(), 5000);
    }
  }

  checkFillLevel() {
    this.fillLevel = this.measureFillLevel();
    if (this.fillLevel >= 80 && !this.emailSent) {
      this.sendEmailAlert();
      this.emailSent = true;
    }
    if (this.fillLevel < 80) {
      this.emailSent = false;
    }
  }

  // ...implementations for connectWiFi, detectPersonNearby, openLid, closeLid, measureFillLevel, sendEmailAlert, updateDisplay...
}
```
