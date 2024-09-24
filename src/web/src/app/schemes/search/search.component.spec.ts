import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SchemeSearchComponent } from './scheme-search.component';

describe('SchemeSearchComponent', () => {
  let component: SchemeSearchComponent;
  let fixture: ComponentFixture<SchemeSearchComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
    imports: [SchemeSearchComponent]
})
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SchemeSearchComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
