const char HEADER[] = "header.xml";
const char FOOTER[] = "</office:text></office:body></office:document>";

const char ITALIC_START_TAG[] = "<text:span text:style-name=\"italic\">";
const char BOLD_START_TAG[] = "<text:span text:style-name=\"bold\">";
const char MONO_START_TAG[] = "<text:span text:style-name=\"mono\">";
const char SCAPS_START_TAG[] = "<text:span text:style-name=\"smallcaps\">";
const char SUP_START_TAG[] = "<text:span text:style-name=\"sup\">";
const char SUB_START_TAG[] = "<text:span text:style-name=\"sub\">";
const char SPAN_END_TAG[] = "</text:span>";
const int  MAX_TITLE_LEVEL = 2;
const char * TITLE_START_TAG[] = {"<text:p text:style-name=\"title1\">",
				"<text:p text:style-name=\"title2\">"};
const char PARAGRAPH_START_TAG[] = "<text:p text:style-name=\"text\">";
const char CODE_START_TAG[] = "<text:p text:style-name=\"code\">";
const char PARAGRAPH_END_TAG[] = "</text:p>\n";
const char LINK_START_TAG_BEGIN[] = "<text:a xlink:type=\"simple\" xlink:href=\"";
const char LINK_START_TAG_END[] = "\">";
const char LINK_END_TAG[] = "</text:a>";
const char SINGLE_QUOTE_START[] = {0xE2, 0x80, 0x98, 0};
const char SINGLE_QUOTE_END[] = {0xE2, 0x80, 0x99, 0};
const char DOUBLE_QUOTE_START[] = {0xE2, 0x80, 0x9C, 0};
const char DOUBLE_QUOTE_END[] = {0xE2, 0x80, 0x9D, 0};
const char NON_BREAKING_SPACE[] = {0xC2, 0xA0, 0};
const char EN_SPACE[] = {0xE2, 0x80, 0x82, 0};
const char LONG_DASH[] = {0xE2, 0x80, 0x93, 0};

