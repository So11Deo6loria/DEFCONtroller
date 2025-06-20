/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#include <stdarg.h>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <texts/TypedTextDatabase.hpp>

uint16_t touchgfx::Font::getStringWidth(const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(TEXT_DIRECTION_LTR, text, pArg);
    va_end(pArg);
    return width;
}

uint16_t touchgfx::Font::getStringWidth(touchgfx::TextDirection textDirection, const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(textDirection, text, pArg);
    va_end(pArg);
    return width;
}

touchgfx::Unicode::UnicodeChar touchgfx::TextProvider::getNextLigature(TextDirection direction)
{
    if (fontGsubTable && nextCharacters.peekChar())
    {
        substituteGlyphs();
        if (nextCharacters.peekChar(1) == 0x093F) // Hindi I-matra
        {
            nextCharacters.replaceAt1(nextCharacters.peekChar());
            nextCharacters.replaceAt0(0x093F);
        }
    }
    return getNextChar();
}

void touchgfx::TextProvider::initializeInternal()
{
    fillInputBuffer();
}

void touchgfx::LCD::drawString(touchgfx::Rect widgetArea, const touchgfx::Rect& invalidatedArea, const touchgfx::LCD::StringVisuals& stringVisuals, const touchgfx::Unicode::UnicodeChar* format, ...)
{
    va_list pArg;
    va_start(pArg, format);
    drawStringLTR(widgetArea, invalidatedArea, stringVisuals, format, pArg);
    va_end(pArg);
}

//Default typed text database
extern const touchgfx::TypedText::TypedTextData* const typedTextDatabaseArray[];

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const touchgfx::Unicode::UnicodeChar texts_all_languages[] TEXT_LOCATION_FLASH_ATTRIBUTE = {
    0x28, 0x63, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x29, 0x20, 0x2d, 0x3e, 0x20, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x20, 0x43, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x0, // @0 "(config) -> Device Config"
    0x28, 0x70, 0x61, 0x79, 0x6c, 0x6f, 0x61, 0x64, 0x29, 0x20, 0x2d, 0x3e, 0x20, 0x41, 0x72, 0x6d, 0x20, 0x50, 0x61, 0x79, 0x6c, 0x6f, 0x64, 0x0, // @26 "(payload) -> Arm Paylod"
    0x28, 0x69, 0x6e, 0x66, 0x6f, 0x29, 0x20, 0x2d, 0x3e, 0x20, 0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x20, 0x49, 0x6e, 0x66, 0x6f, 0x0, // @50 "(info) -> Device Info"
    0x28, 0x68, 0x65, 0x6c, 0x70, 0x29, 0x20, 0x2d, 0x3e, 0x20, 0x48, 0x65, 0x6c, 0x70, 0x20, 0x4d, 0x65, 0x6e, 0x75, 0x0, // @72 "(help) -> Help Menu"
    0x42, 0x52, 0x4f, 0x55, 0x47, 0x48, 0x54, 0x20, 0x54, 0x4f, 0x20, 0x59, 0x4f, 0x55, 0x20, 0x42, 0x59, 0x3a, 0x0, // @92 "BROUGHT TO YOU BY:"
    0x46, 0x69, 0x72, 0x6d, 0x77, 0x61, 0x72, 0x65, 0x20, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x3a, 0x0, // @111 "Firmware Version:"
    0x4d, 0x69, 0x6c, 0x65, 0x73, 0x20, 0x74, 0x6f, 0x20, 0x45, 0x6d, 0x70, 0x74, 0x79, 0x3a, 0x0, // @129 "Miles to Empty:"
    0x50, 0x75, 0x6d, 0x70, 0x20, 0x4f, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x0, // @145 "Pump Operation:"
    0x40, 0x53, 0x6f, 0x31, 0x31, 0x44, 0x65, 0x6f, 0x36, 0x6c, 0x6f, 0x72, 0x69, 0x61, 0x0, // @161 "@So11Deo6loria"
    0x43, 0x4f, 0x4d, 0x20, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x66, 0x61, 0x63, 0x65, 0x0, // @176 "COM Interface"
    0x4e, 0x6f, 0x74, 0x20, 0x49, 0x6e, 0x69, 0x74, 0x69, 0x61, 0x74, 0x65, 0x64, 0x0, // @190 "Not Initiated"
    0x4f, 0x69, 0x6c, 0x20, 0x4c, 0x69, 0x66, 0x65, 0x20, 0x28, 0x25, 0x29, 0x3a, 0x0, // @204 "Oil Life (%):"
    0x40, 0x6f, 0x63, 0x72, 0x6f, 0x75, 0x73, 0x65, 0x32, 0x30, 0x32, 0x30, 0x0, // @218 "@ocrouse2020"
    0x42, 0x48, 0x56, 0x5f, 0x46, 0x4c, 0x41, 0x47, 0x5f, 0x45, 0x58, 0x0, // @231 "BHV_FLAG_EX"
    0x43, 0x61, 0x6e, 0x20, 0x45, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x0, // @243 "Can Example"
    0x44, 0x65, 0x62, 0x75, 0x67, 0x20, 0x4d, 0x6f, 0x64, 0x65, 0x3a, 0x0, // @255 "Debug Mode:"
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x0, // @267 "1234567890"
    0x48, 0x55, 0x44, 0x20, 0x53, 0x74, 0x61, 0x74, 0x65, 0x3a, 0x0, // @278 "HUD State:"
    0x40, 0x6d, 0x64, 0x6f, 0x6d, 0x61, 0x6e, 0x69, 0x63, 0x0, // @289 "@mdomanic"
    0x4c, 0x69, 0x73, 0x74, 0x20, 0x49, 0x74, 0x65, 0x6d, 0x0, // @299 "List Item"
    0x4f, 0x64, 0x6f, 0x6d, 0x65, 0x74, 0x65, 0x72, 0x3a, 0x0, // @309 "Odometer:"
    0x43, 0x68, 0x65, 0x63, 0x6b, 0x69, 0x6e, 0x67, 0x0, // @319 "Checking"
    0x44, 0x69, 0x73, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x0, // @328 "Disabled"
    0x49, 0x6e, 0x61, 0x63, 0x74, 0x69, 0x76, 0x65, 0x0, // @337 "Inactive"
    0x31, 0x2e, 0x30, 0x2e, 0x32, 0x33, 0x33, 0x0, // @346 "1.0.233"
    0x2, 0x0, // @354 "<>"
    0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x64, 0x0, // @356 "Enabled"
    0x41, 0x63, 0x74, 0x69, 0x76, 0x65, 0x0, // @364 "Active"
    0x53, 0x48, 0x49, 0x45, 0x4c, 0x44, 0x0, // @371 "SHIELD"
    0x38, 0x39, 0x31, 0x30, 0x39, 0x0, // @378 "89109"
    0x41, 0x72, 0x6d, 0x65, 0x64, 0x0, // @384 "Armed"
    0x50, 0x69, 0x6e, 0x3a, 0x20, 0x0, // @390 "Pin: "
    0x52, 0x45, 0x63, 0x6f, 0x6e, 0x0, // @396 "REcon"
    0x32, 0x30, 0x30, 0x35, 0x0, // @402 "2005"
    0x42, 0x61, 0x63, 0x6b, 0x0, // @407 "Back"
    0x48, 0x6f, 0x6d, 0x65, 0x0, // @412 "Home"
    0x20, 0x33, 0x31, 0x0, // @417 " 31"
    0x2d, 0x20, 0x2d, 0x0, // @421 "- -"
    0x30, 0x2e, 0x33, 0x0, // @425 "0.3"
    0x2, 0x46, 0x0, // @429 "<>F"
    0x4f, 0x66, 0x66, 0x0, // @432 "Off"
    0x54, 0x42, 0x44, 0x0, // @436 "TBD"
    0x2d, 0x2d, 0x0, // @440 "--"
    0x34, 0x32, 0x0, // @443 "42"
    0x41, 0x63, 0x0, // @446 "Ac"
    0x64, 0x6c, 0x0, // @449 "dl"
    0x6d, 0x67, 0x0 // @452 "mg"
};
TEXT_LOCATION_FLASH_PRAGMA
KEEP extern uint32_t const indicesGb[] TEXT_LOCATION_FLASH_ATTRIBUTE;

//array holding dynamically installed languages
struct TranslationHeader
{
    uint32_t offset_to_texts;
    uint32_t offset_to_indices;
    uint32_t offset_to_typedtext;
};
static const TranslationHeader* languagesArray[1] = { 0 };

//Compiled and linked in languages
static const uint32_t* const staticLanguageIndices[] = {
    indicesGb
};

touchgfx::LanguageId touchgfx::Texts::currentLanguage = static_cast<touchgfx::LanguageId>(0);
static const touchgfx::Unicode::UnicodeChar* currentLanguagePtr = 0;
static const uint32_t* currentLanguageIndices = 0;

void touchgfx::Texts::setLanguage(touchgfx::LanguageId id)
{
    const touchgfx::TypedText::TypedTextData* currentLanguageTypedText = 0;
    if (id < 1)
    {
        if (languagesArray[id] != 0)
        {
            // Dynamic translation is added
            const TranslationHeader* translation = languagesArray[id];
            currentLanguagePtr = (const touchgfx::Unicode::UnicodeChar*)(((const uint8_t*)translation) + translation->offset_to_texts);
            currentLanguageIndices = (const uint32_t*)(((const uint8_t*)translation) + translation->offset_to_indices);
            currentLanguageTypedText = (const touchgfx::TypedText::TypedTextData*)(((const uint8_t*)translation) + translation->offset_to_typedtext);
        }
        else
        {
            // Compiled and linked in languages
            currentLanguagePtr = texts_all_languages;
            currentLanguageIndices = staticLanguageIndices[id];
            currentLanguageTypedText = typedTextDatabaseArray[id];
        }
    }

    if (currentLanguageTypedText)
    {
        currentLanguage = id;
        touchgfx::TypedText::registerTypedTextDatabase(currentLanguageTypedText,
                                                       TypedTextDatabase::getFonts(), TypedTextDatabase::getInstanceSize());
    }
}

void touchgfx::Texts::setTranslation(touchgfx::LanguageId id, const void* translation)
{
    languagesArray[id] = (const TranslationHeader*)translation;
}

const touchgfx::Unicode::UnicodeChar* touchgfx::Texts::getText(TypedTextId id) const
{
    return &currentLanguagePtr[currentLanguageIndices[id]];
}
