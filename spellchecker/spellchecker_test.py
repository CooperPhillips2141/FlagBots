import autocorrect as ac

# create a dictionary based on the words we want to use
# word maps to frequency
word_dict = {
    "dance":0.25,
    "act":0.25,
    "hi":0.25,
    "hello":0.25
}

# make the autocorrect use our dictionary
spell = ac.Speller(nlp_data=word_dict)
print(spell("dancer donce acr ho hi"))
print("autocorrected")
