// Probably we can add a reward type (like on PS3)
typedef struct achievement_s
{
	char* name;
	char* code;
	char* description;
}achievement_t;

// Some small struct for displaying onscreen.
typedef struct reward_s
{
	int rewardCode;
	int startTime;
	const char* rewardString;
	const char* rewardDescription;
}reward_t;


#define ACHIEVEMENT_COUNT 49

achievement_t achievements[ACHIEVEMENT_COUNT];

void buildAchievementList()
{
	if(achievements[0].name)
		return;

	achievements[0].name = "Back in the Saddle";
	achievements[0].code = "BACK_IN_THE_SADDLE";
	achievements[0].description = "Help train the local militia.";

	achievements[1].name = "Danger Close";
	achievements[1].code = "DANGER_CLOSE";
	achievements[1].description = "Get hand picked for Shepherd's elite squad.";

	achievements[2].name = "Cold Shoulder";
	achievements[2].code = "COLD_SHOULDER";
	achievements[2].description = "Infiltrate the snowy mountain side base.";

	achievements[3].name = "Tag 'em and bag 'em";
	achievements[3].code = "TAGEM_AND_BAGEM";
	achievements[3].description = "Find Rojas in the Favelas.";

	achievements[4].name = "Royale with Cheese";
	achievements[4].code = "ROYAL_WITH_CHEESE";
	achievements[4].description = "Defend Burger Town.";

	achievements[5].name = "Soap on a Rope";
	achievements[5].code = "SOAP_ON_A_ROPE";
	achievements[5].description = "Storm the gulag.";

	achievements[6].name = "Desperate Times";
	achievements[6].code = "DESPERATE_TIMES";
	achievements[6].description = "Execute the plan to help the Americans.";

	achievements[7].name = "Whiskey Hotel";
	achievements[7].code = "HOUSTON_WE_HAVE_A_PROBLEM";
	achievements[7].description = "Take back Whiskey Hotel.";

	achievements[8].name = "The Pawn";
	achievements[8].code = "THE_PAWN";
	achievements[8].description = "Assault Makarov's safe house.";

	achievements[8].name = "Out of the Frying Pan...";
	achievements[8].code = "OUT_OF_THE_FRYING_PAN";
	achievements[8].description = "Complete the mission in the airplane graveyard.";

	achievements[9].name = "For the Record";
	achievements[9].code = "FOR_THE_RECORD";
	achievements[9].description = "Complete the Single Player campaign on any difficulty.";

	achievements[10].name = "The Price of War";
	achievements[10].code = "THE_PRICE_OF_WAR";
	achievements[10].description = "Complete the single player campaign on Hardened or Veteran Difficulty.";

	achievements[11].name = "First Day of School";
	achievements[11].code = "FIRST_DAY_OF_SCHOOL";
	achievements[11].description = "Complete 'S.S.D.D' and 'Team Player' on Veteran Difficulty.";

	achievements[12].name = "Black Diamond";
	achievements[12].code = "BLACK_DIAMOND";
	achievements[12].description = "Complete 'Cliffhanger' on Veteran Difficulty.";

	achievements[13].name = "Turistas";
	achievements[13].code = "TURISTAS";
	achievements[13].description = "Complete 'Takedown' and 'The Hornet's Nest' on Veteran Difficulty.";

	achievements[14].name = "Red Dawn";
	achievements[14].code = "RED_DAWN";
	achievements[14].description = "Complete 'Wolverines!' and 'Exodus' on Veteran Difficulty.";

	achievements[15].name = "Prisoner #627";
	achievements[15].code = "PRISONER_627";
	achievements[15].description = "Complete 'The Only Easy Day... Was Yesterday' and 'The Gulag' on Veteran Difficulty.";

	achievements[16].name = "Ends Justify the Means";
	achievements[16].code = "ENDS_JUSTIFY_THE_MEANS";
	achievements[16].description = "Complete 'Contingency' on Veteran Difficulty";

	achievements[17].name = "Homecoming";
	achievements[17].code = "HOME_COMING";
	achievements[17].description = "Complete 'Of Their Own Accord', 'Second Sun', and 'Whiskey Hotel' on Veteran Difficulty.";

	achievements[18].name = "Queen takes Rook";
	achievements[18].code = "QUEEN_TAKES_ROOK";
	achievements[18].description = "Complete 'Loose Ends' and 'The Enemy of My Enemy' on Veteran Difficulty.";

	achievements[19].name = "Off the Grid";
	achievements[19].code = "OFF_THE_GRID";
	achievements[19].description = "Complete 'Just Like Old Times' and 'Endgame' on Veteran Difficulty.";

	achievements[20].name = "Pit Boss";
	achievements[20].code = "PIT_BOSS";
	achievements[20].description = "Run The Pit in 'S.S.D.D' and finish with a final time under 30 seconds.";

	achievements[21].name = "Ghost";
	achievements[21].code = "GHOST";
	achievements[21].description = "Plant the C4 in 'Cliffhanger' without alerting or injuring anyone in the blizzard.";

	achievements[22].name = "Colonel Sanderson";
	achievements[22].code = "COLONEL_SANDERSON";
	achievements[22].description = "Kill 7 chickens in under 10 seconds in 'The Hornet's Nest'.";

	achievements[23].name = "Gold Star";
	achievements[23].code = "GOLD_STAR";
	achievements[23].description = "Earn 1 star in Special Ops.";

	achievements[24].name = "Hotel Bravo";
	achievements[24].code = "HOTEL_BRAVO";
	achievements[24].description = "Earn 4 stars in Special Ops.";

	achievements[25].name = "Charlie On Our Six";
	achievements[25].code = "CHARLIE_ON_OUR_SIX";
	achievements[25].description = "Earn 8 stars in Special Ops.";

	achievements[26].name = "It Goes to Eleven";
	achievements[26].code = "IT_GOES_TO_ELEVEN";
	achievements[26].description = "Earn at least 1 star in 11 different Special Op missions.";

	achievements[27].name = "Operational Asset";
	achievements[27].code = "OPERATIONAL_ASSET";
	achievements[27].description = "Earn all 3 stars in at least 5 different Special Op missions.";

	achievements[28].name = "Blackjack";
	achievements[28].code = "BLACKJACK";
	achievements[28].description = "Earn 21 stars in Special Ops.";

	achievements[29].name = "Honor Roll";
	achievements[29].code = "HONOR_ROLL";
	achievements[29].description = "Earn at least 1 star in each Special Op mission.";

	achievements[30].name = "Operative";
	achievements[30].code = "OPERATIVE";
	achievements[30].description = "Earn all 3 stars in at least 10 different Special Op missions.";

	achievements[31].name = "Specialist";
	achievements[31].code = "SPECIALIST";
	achievements[31].description = "Earn 30 stars in Special Ops.";

	achievements[32].name = "Professional";
	achievements[32].code = "PROFESSIONAL";
	achievements[32].description = "Earn all 3 stars in at least 15 different Special Op missions.";

	achievements[33].name = "Star 69";
	achievements[33].code = "STAR_69";
	achievements[33].description = "Earn 69 stars in Special Ops.";

	achievements[34].name = "Three-some";
	achievements[34].code = "THREESOME";
	achievements[34].description = "Kill at least 3 enemies with a single shot from a grenade launcher in Single Player or Special Ops.";

	achievements[35].name = "Downed but Not Out";
	achievements[35].code = "DOWNED_BUT_NOT_OUT";
	achievements[35].description = "Kill 4 enemies in a row while downed in Special Ops.";

	achievements[36].name = "No Rest For the Wary";
	achievements[36].code = "NO_REST_FOR_THE_WARY";
	achievements[36].description = "Knife an enemy without him ever knowing you were there in Single Player or Special Ops.";

	achievements[37].name = "I'm the Juggernaut...";
	achievements[37].code = "IM_THE_JUGGERNAUT";
	achievements[37].description = "Kill a Juggernaut in Special Ops.";

	achievements[38].name = "Desperado";
	achievements[38].code = "ONE_MAN_ARMY";
	achievements[38].description = "Kill 5 enemies in a row using 5 different weapons or attachments in Single Player or Special Ops.";

	achievements[39].name = "Ten plus foot-mobiles";
	achievements[39].code = "TEN_PLUS_FOOT_MOBILES";
	achievements[39].description = "Kill at least 10 enemies with one Predator missile in Single Player or Special Ops.";

	achievements[40].name = "Unnecessary Roughness";
	achievements[40].code = "UNNECESSARY_ROUGHNESS";
	achievements[40].description = "Use a riot shield to beat down an enemy in Single Player or Special Ops.";

	achievements[41].name = "Knock-knock";
	achievements[41].code = "KNOCK_KNOCK";
	achievements[41].description = "Kill 4 enemies with 4 shots during a slow-mo breach in Single Player or Special Ops.";

	achievements[42].name = "Look Ma Two Hands";
	achievements[42].code = "LOOK_MA_TWO_HANDS";
	achievements[42].description = "Kill 10 enemies in a row using akimbo weapons in Single Player or Special Ops.";

	achievements[43].name = "Some Like it Hot";
	achievements[43].code = "SOME_LIKE_IT_HOT";
	achievements[43].description = "Kill 6 enemies in a row using a thermal weapon in Single Player or Special Ops.";

	achievements[44].name = "Two Birds with One Stone";
	achievements[44].code = "TWO_BIRDS_WITH_ONE_STONE";
	achievements[44].description = "Kill 2 enemies with a single bullet in Single Player or Special Ops.";

	achievements[45].name = "The Road Less Traveled";
	achievements[45].code = "THE_ROAD_LESS_TRAVELED";
	achievements[45].description = "Collect 22 enemy intel items.";

	achievements[46].name = "Leave No Stone Unturned";
	achievements[46].code = "LEAVE_NO_STONE_UNTURNED";
	achievements[46].description = "Collect 45 enemy intel items.";

	achievements[47].name = "Drive By";
	achievements[47].code = "DRIVE_BY";
	achievements[47].description = "Kill 20 enemies in a row while driving a vehicle in Single Player or Special Ops.";

	achievements[48].name = "The Harder They Fall";
	achievements[48].code = "THE_HARDER_THEY_FALL";
	achievements[48].description = "Kill 2 rappelling enemies in a row before they land on their feet in Single Player or Special Ops.";
}