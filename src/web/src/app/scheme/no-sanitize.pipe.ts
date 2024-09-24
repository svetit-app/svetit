import { Pipe, PipeTransform, inject } from '@angular/core';
import { DomSanitizer, SafeHtml } from '@angular/platform-browser';

@Pipe({
    name: 'noSanitize',
    standalone: true
})
export class NoSanitizePipe implements PipeTransform {
   private domSanitizer = inject(DomSanitizer);

   transform(html: string): SafeHtml {
      return this.domSanitizer.bypassSecurityTrustHtml(html);
   }
}

