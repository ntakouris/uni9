typedef volatile struct _PIO
{
    unsigned int PER;          // PIO Enable register ( Write−only ) 
    unsigned int PDR;          // PIO Disable register ( Write−only ) 
    unsigned int PSR;          // PIO Status register ( Read−only ) hb
    unsigned int Reserved0;    // Unused register 
    unsigned int OER;          // Output Enable register ( Write−only ) 
    unsigned int ODR;          // Output Disable register ( Write−only ) 
    unsigned int OSR;          // Output Status register ( Read−only ) 
    unsigned int Reserved1;    // Unused register 
    unsigned int IFER;         // glitch Input f i l t e r Enable ( Write−only ) 
    unsigned int IFDR;         // glitch Input f i l t e r Disable ( Write−only ) 
    unsigned int IFSR;         // glitch Input f i l t e r Status ( Read−only ) 
    unsigned int Reserved2;    // Unused register 
    unsigned int SODR;         // Set Output Data register ( Write−only ) 
    unsigned int CODR;         // Clear Output Data register ( Write−only ) 
    unsigned int ODSR;         // Output Data Status register ( Read−only ) 
    unsigned int PDSR;         // Pin Data Status register ( Read−only ) 
    unsigned int IER;          // interrupt Enable register ( Write−only ) 
    unsigned int IDR;          // interrupt Disable register ( Write−only ) 
    unsigned int IMR;          // interrupt Mask register ( Write−only ) 
    unsigned int ISR;          // interrupt Status register ( Read−only ) 
    unsigned int MDER;         // Multi−driver Enable ( Write−only ) 
    unsigned int MDDR;         // Multi−driver Disable ( Write−only ) 
    unsigned int MDSR;         // Multi−driver Status ( Read−only ) 
    unsigned int Reserved3;    // Unused register 
    unsigned int PUDR;         // pull−up Disable register ( Write−only ) 
    unsigned int PUER;         // pull−up Enable register ( Write−only ) 
    unsigned int PUSR;         // pull−up Status register ( Read−only ) 
    unsigned int Reserved4;    // Unused register 
    unsigned int ASR;          // peripheral A select ( Write−only ) 
    unsigned int BSR;          // peripheral B select ( Write−only ) 
    unsigned int ABSR;         // peripheral AB Status ( Read−only ) 
    unsigned int Reserved5[9]; // Unused register 
    unsigned int OWER;         // Output write enable ( Write−only ) 
    unsigned int OWDR;         // Output write disable ( Write−only ) 
    unsigned int OWSR;         // Output write Status ( Read−only ) 
} PIO;
typedef volatile struct _AIC
{
    unsigned int SMR[32];      // Source mode register ( Read−Write ) 
    unsigned int SVR[32];      // Source vector register ( Read−Write ) 
    unsigned int IVR;          // interrupt vector register ( Read−only ) 
    unsigned int FVR;          // Fast interrupt vector register ( Read−only ) 
    unsigned int ISR;          // interrupt status register ( Read−only ) 
    unsigned int IPR;          // interrupt pending register ( Read−only ) 
    unsigned int IMR;          // interrupt mask register ( Read−only ) 
    unsigned int CISR;         // Core interrupt status register ( Read−only ) 
    unsigned int Reserved1[2]; // Unused register 
    unsigned int IECR;         // interrupt enable command register ( Write−only ) 
    unsigned int IDCR;         // interrupt disable command register ( Write−only ) 
    unsigned int ICCR;         // interrupt clear command register ( Write−only ) 
    unsigned int ISCR;         // interrupt set command register ( Write−only ) 
    unsigned int EICR;         // End of interrupt command register ( Write−only ) 
    unsigned int SPUR;         // Spurious interrupt vector register ( Read−Write ) 
    unsigned int DCR;          // Debug control register ( Read−Write ) 
    unsigned int Reserved2;    // Unused register 
    unsigned int FFER;         // Fast forcing enable register ( Write−only ) 
    unsigned int FFDR;         // Fast forcing disable register ( Write−only ) 
    unsigned int FFSR;         // Fast forcing status register ( Write−only ) 
} AIC;
typedef volatile struct _TCCHAN
{
    unsigned int CCR;          // Channel control register ( Write−only ) 
    unsigned int CMR;          // Channel Mode register ( Read−Write ) 
    unsigned int Reserved1[2]; // Unused register 
    unsigned int CV;           // Counter Value ( Read−only ) 
    unsigned int RA;           // register A ( Read−Write ) 
    unsigned int RB;           // register B ( Read−Write ) 
    unsigned int RC;           // register C ( Read−Write ) 
    unsigned int SR;           // Status register ( Read−only ) 
    unsigned int IER;          // interrupt Enable register ( Write−only ) 
    unsigned int IDR;          // interrupt Disable register ( Write−only ) 
    unsigned int IMR;          // interrupt Mask register ( Read−only ) 
    unsigned int Reserved2[4]; // Unused register 
} TCCHAN;
typedef volatile struct _TC
{
    TCCHAN Channel_0;
    TCCHAN Channel_1;
    TCCHAN Channel_2;
    unsigned int BCR; // Block control register ( Write−only ) 
    unsigned int BMR; // Block Mode register ( Read−Write ) 
} TC;