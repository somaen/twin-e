int InitAnim(char costume, short int arg_4, unsigned char arg_8, short int actorNum);
void loadGfxSub(unsigned char *ptr);
int setAnimAtKeyFrame(int keyframeIdx, unsigned char *anim, unsigned char *body, animTimerData *animTimerDataPtr);
int SetInterAnimObjet2(int animState, char *animData, char *body, animTimerData *animTimerDataPtr);
int SetInterAnimObjet(int animState, char *animData, char *body, animTimerData *animTimerDataPtr);
int GetNbFramesAnim(char *ptr);
int GetBouclageAnim(char *ptr);
int StockInterAnim(char *lBufAnim, char *lBody, animTimerData *animTimerDataPtr);
int SetInterDepObjet(int position, char *anim, char *body, animTimerData *animTimerDataPtr);
void PatchInterAngle(char **ptr, int bp, int bx);
void PatchInterStep(char **ptr, int bp, int bx);
int PatchType(char **ptr);
