# md2fodt

## Presentation

md2fodt is a simple, blazingly fast Markdown to flat ODT converter. It uses character-by-character file streaming and unlocked IOs to reach maximal performances.

## Notes on syntax

md2fodt accepts an extended Marddown syntax :
  * Hashes for titles and subtitles
  * \* *italic* \*
  * \*\* **bold** \*\*
  * |<code>monotype</code>|
  * 21&lt;<sup>st</sup>&gt;
  * CO&gt;<sub>2</sub>&lt;
  * + SMALL CAPS +
  * ~<code>Code block with different margin and alignment</code>

## Special features

md2fodt also implements smart dashes and quotes, and adds non-breaking spaces in french text when needed. Other parameters, like page size or font faces, can be modified in the <mono>header.xml</code> file.
