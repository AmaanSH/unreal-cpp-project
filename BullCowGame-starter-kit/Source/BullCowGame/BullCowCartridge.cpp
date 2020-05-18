// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();    

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{ 
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You guessed the right word!"));
        EndGame();
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine("No repeating letters, guess again");
        return;
    }

    if (Guess.Len() != HiddenWord.Len() && Lives > 0)
    {
        PrintLine(TEXT("The word is %i characters.\nYou have %i lives! Try Again!"), HiddenWord.Len(), Lives);
        return;
    }

    --Lives;
    PrintLine(TEXT("Lost Life!"));

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You ran out of lives!"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    
    PrintLine(TEXT("CHOSEN WORD: %s"), *HiddenWord); // debug

    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    // Welcome the Player
    PrintLine(TEXT("Welcome to the Bull Cows Game!"));
    PrintLine(TEXT("Guess the word!"), HiddenWord.Len());
    PrintLine(TEXT("It is %i characters long."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);

    // Prompt Player For Guess
    PrintLine(TEXT("Please enter a word and press ENTER:"));
}

bool UBullCowCartridge::IsIsogram(const FString& Guess) const
{
    for (int32 Index = 0; Index < Guess.Len(); Index++)
    {
        for (int32 Comparitor = Index + 1; Comparitor < Guess.Len(); Comparitor++)
        {
            if (Guess[Index] == Guess[Comparitor])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        
        for (int32 WordIndex = 0; WordIndex < HiddenWord.Len(); WordIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[WordIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nGame Over! Press ENTER to play again"));
}