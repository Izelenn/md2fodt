#include <stdio.h>

const char HEADER[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><office:document xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\" xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\" office:version=\"1.2\" office:mimetype=\"application/vnd.oasis.opendocument.text\"><office:font-face-decls><style:font-face style:name=\"Serif\" svg:font-family=\"&apos;URW Palladio L&apos;\"/><style:font-face style:name=\"Mono\" svg:font-family=\"&apos;FreeMono&apos;\"/></office:font-face-decls><office:automatic-styles><style:style style:name=\"title1\" style:family=\"paragraph\"><style:paragraph-properties fo:text-align=\"left\"/><style:text-properties style:font-name=\"Serif\" fo:font-size=\"18pt\" fo:font-weight=\"bold\"/></style:style><style:style style:name=\"title2\" style:family=\"paragraph\"><style:paragraph-properties fo:text-align=\"left\"/><style:text-properties style:font-name=\"Serif\" fo:font-size=\"18pt\" fo:font-style=\"italic\"/></style:style><style:style style:name=\"text\" style:family=\"paragraph\"><style:paragraph-properties fo:text-align=\"justify\" fo:text-indent=\"0.2in\" fo:orphans=\"2\" fo:widows=\"2\"/><style:text-properties style:font-name=\"Serif\" fo:font-size=\"10pt\" fo:language=\"fr\"/></style:style><style:style style:name=\"code\" style:family=\"paragraph\"><style:paragraph-properties fo:text-align=\"left\" fo:text-indent=\"0in\" fo:orphans=\"2\" fo:widows=\"2\"/><style:text-properties style:font-name=\"Mono\" fo:font-size=\"10pt\" fo:language=\"fr\"/></style:style><style:style style:name=\"Footer\" style:family=\"paragraph\" style:class=\"extra\"><style:paragraph-properties fo:text-align=\"center\"/><style:text-properties style:font-name=\"Serif\" fo:font-size=\"10pt\"/></style:style><style:style style:name=\"italic\" style:family=\"text\"><style:text-properties fo:font-style=\"italic\"/></style:style><style:style style:name=\"bold\" style:family=\"text\"><style:text-properties fo:font-weight=\"bold\"/></style:style><style:style style:name=\"mono\" style:family=\"text\"><style:text-properties style:font-name=\"Mono\"/></style:style><style:style style:name=\"smallcaps\" style:family=\"text\"><style:text-properties fo:font-variant=\"small-caps\"/></style:style><style:style style:name=\"sup\" style:family=\"text\"><style:text-properties style:text-position=\"super 58%\"/></style:style><style:style style:name=\"sub\" style:family=\"text\"><style:text-properties style:text-position=\"sub 58%\"/></style:style><style:page-layout style:name=\"page\"><style:page-layout-properties fo:page-width=\"6in\" fo:page-height=\"9in\" style:num-format=\"1\" style:print-orientation=\"portrait\" fo:margin=\"0.6in\" fo:margin-top=\"0.55in\" fo:margin-bottom=\"0.4in\" fo:margin-left=\"0.6in\" fo:margin-right=\"0.6in\" style:writing-mode=\"lr-tb\" style:footnote-max-height=\"0cm\"/><style:footer-style><style:header-footer-properties fo:min-height=\"0.15in\" fo:margin-left=\"0in\" fo:margin-right=\"0in\" fo:margin-top=\"0.1in\" style:dynamic-spacing=\"false\"/></style:footer-style></style:page-layout></office:automatic-styles><office:master-styles><style:master-page style:name=\"Standard\" style:page-layout-name=\"page\"><style:footer><text:p text:style-name=\"Footer\">-<text:span text:style-name=\"text\"><text:page-number text:select-page=\"current\"/></text:span>-</text:p></style:footer></style:master-page></office:master-styles><office:body><office:text text:use-soft-page-breaks=\"true\">";
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
const char SINGLE_QUOTE_START[] = {0xE2, 0x80, 0x98, 0};
const char SINGLE_QUOTE_END[] = {0xE2, 0x80, 0x99, 0};
const char DOUBLE_QUOTE_START[] = {0xE2, 0x80, 0x9C, 0};
const char DOUBLE_QUOTE_END[] = {0xE2, 0x80, 0x9D, 0};
const char NON_BREAKING_SPACE[] = {0xC2, 0xA0, 0};
const char LONG_DASH[] = {0xE2, 0x80, 0x93, 0};

int is_special(char c) {
	if ((c == '!') || (c == '?') || (c == ':') || (c == ';'))
		return 1;
	return 0;
}

void process(FILE * input, FILE * output) {
	struct {
		int italic;
		int bold;
		int mono;
		int smallcaps;
		int sub;
		int sup;
		int title_level;
		int double_quote;
		int single_quote;
		int last_was_break;
		int last_was_space;
		int paragraph;
		int ignore_next;
	} state = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char current, next;
	// Copy XML header
	fputs_unlocked(HEADER, output);
	// Remove UTF-8 garbage
	while ((current = (char)fgetc_unlocked(input)) != '#');
	// Processing loop
	while ((next = (char)fgetc_unlocked(input)) != EOF) {
		if (state.ignore_next) {
			state.ignore_next = 0;
			current = next;
			continue;
		}
		switch (current) {
		case '*' :	// Italic & bold
			if (next == '*') {
				state.bold = state.bold ? 0 : 1;
				fputs_unlocked(state.bold ? SPAN_END_TAG : BOLD_START_TAG, output);
				state.ignore_next = 1;
			} else {
				fputs_unlocked(state.italic ? SPAN_END_TAG : ITALIC_START_TAG, output);
				state.italic = state.italic ? 0 : 1;
			}
			break;
		case '|' :	// Monotype
			fputs_unlocked(state.mono ? SPAN_END_TAG : MONO_START_TAG, output);
			state.mono = state.mono ? 0 : 1;
			break;
		case '+' :	// Small caps
			fputs_unlocked(state.smallcaps ? SPAN_END_TAG : SCAPS_START_TAG, output);
			state.smallcaps = state.smallcaps ? 0 : 1;
			break;
		case '<' :	// Start of superscript or end of subscript
			fputs_unlocked(state.sub ? SPAN_END_TAG : SUP_START_TAG, output);
			if (state.sub)
				state.sub = 0;
			else state.sup = 1;
			break;
		case '>' :	// Start of subscript or end of superscript
			fputs_unlocked(state.sup ? SPAN_END_TAG : SUB_START_TAG, output);
			if (state.sup)
				state.sup = 0;
			else state.sub = 1;
			break;
		case '#' :	// Title
			if (state.title_level < MAX_TITLE_LEVEL)
				state.title_level += 1;
			if (next != '#') {
				fputs_unlocked(TITLE_START_TAG[state.title_level - 1], output);
				state.paragraph = 1;
			}
			if (next == ' ')
				state.ignore_next = 1;
			break;
		case '~' :	// Code block
			if (!state.paragraph) {
				fputs_unlocked(CODE_START_TAG, output);
				state.paragraph = 1;
			}
			break;
		case '\n' :	// Line break
			state.last_was_break = 1;
			for (int i = 0 ; i < state.mono + state.bold + state.italic + state.smallcaps ; i++)
				fputs_unlocked(SPAN_END_TAG, output);
			fputs_unlocked(PARAGRAPH_END_TAG, output);
			state.paragraph = 0;
			state.title_level = 0;
			if ((next != '#') && (next != '~')) {
				fputs_unlocked(PARAGRAPH_START_TAG, output);
				state.paragraph = 1;
				if (state.italic)
					fputs_unlocked(ITALIC_START_TAG, output);
				if (state.bold)
					fputs_unlocked(BOLD_START_TAG, output);
				if (state.mono)
					fputs_unlocked(MONO_START_TAG, output);
				if (state.smallcaps)
					fputs_unlocked(SCAPS_START_TAG, output);
			}
			break;
		case ' ' :	// Space
			state.last_was_space = 1;
			if (is_special(next))
				fputs_unlocked(NON_BREAKING_SPACE, output);
			else fputc_unlocked(' ', output);
			break;
		case '\'' :	// Single quote
			fputs_unlocked(state.single_quote ? SINGLE_QUOTE_END : SINGLE_QUOTE_START, output);
			state.single_quote = state.single_quote ? 0 : 1;
			break;
		case '\"' :	// Double quote
			fputs_unlocked(state.double_quote ? DOUBLE_QUOTE_END : DOUBLE_QUOTE_START, output);
			state.double_quote = state.double_quote ? 0 : 1;
			break;
		case '&' :	// Just for XML compliance
			fputs_unlocked("&amp;", output);
			break;
		case '-' :	// Smart dashes
			if (state.last_was_space)
				fputs_unlocked(LONG_DASH, output);
			else if (state.last_was_break) {
				fputs_unlocked(LONG_DASH, output);
				fputs_unlocked(NON_BREAKING_SPACE, output);
				state.ignore_next = 1;
			}
			else fputc_unlocked('-', output);
			break;
		case '\\' :
			fputc_unlocked(next, output);
			state.ignore_next = 1;
			break;
		default:
			fputc_unlocked(current, output);
			state.last_was_space = 0;
			state.last_was_break = 0;
		}
		if ((next == '\'') && (!(state.last_was_break || state.last_was_space)))
			state.single_quote = 1;
		current = next;
	}
	fputc_unlocked(current, output);
	if (state.paragraph)
		fputs_unlocked(PARAGRAPH_END_TAG, output);
	fputs_unlocked(FOOTER, output);
}

int main(int argc, char ** argv) {
	if (argc != 2) {
		puts("Usage : md2fodt <filename>");
		return 1;
	}
	FILE * input = fopen(argv[1], "rb");
	if (!input) {
		puts("Unable to open input file");
		return 2;
	}
	char filename[256];
	sprintf(filename, "%s.fodt", argv[1]);
	FILE * output = fopen(filename, "wb");
	if (!output) {
		puts("Unable to create output file");
		fclose(input);
		return 3;
	}
	process(input, output);
	fclose(output);
	fclose(input);
	puts("Done processing");
	return 0;
}

