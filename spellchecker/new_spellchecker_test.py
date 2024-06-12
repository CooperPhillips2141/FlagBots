from spellchecker import SpellChecker

# Initialize the spellchecker without a default language
spell = SpellChecker(language=None)

# Custom dictionary words
custom_words = ["dance", "hi", "hello", "mirror"]

# Load the custom words into the spellchecker
spell.word_frequency.load_words(custom_words)

# Word to check
word_to_check = "amirrio"

# Find the best correction
corrected_word = spell.correction(word_to_check)
print(f"Corrected word: {corrected_word}")

# Get a list of candidate corrections
suggestions = spell.candidates(word_to_check)
print(f"Suggestions: {suggestions}")